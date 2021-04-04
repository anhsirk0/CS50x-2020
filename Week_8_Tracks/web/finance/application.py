import os

# from cs50 import SQL
import sqlite3
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, usd, lookup, get_time

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
# db = SQL("sqlite:///finance.db")
dbfile = 'finance.db'

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")

api_key = os.environ.get("API_KEY")

@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    username = session["user_id"]
    conn = sqlite3.connect(dbfile)
    db = conn.cursor()
    q2 = db.execute(f"SELECT cash FROM users WHERE username = '{username}'")
    cash = q2.fetchone()[0]
    q = db.execute(f"SELECT * FROM shares WHERE username = '{username}'")
    rows = q.fetchall()
    spent = 0
    for row in rows:
        spent += row[3]*row[4]
    total_cash = round(cash + spent, 2)
    return render_template("portfolio.html",rows=rows,cash=cash,total_cash=total_cash)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    username = session["user_id"]

    if request.method == "GET":
        return render_template("buy.html")

    # else post
    symbol = request.form.get("symbol")
    shares = int(request.form.get("shares"))
    result = lookup(symbol)
    if not result:
        return apology("Invalid Symbol")
    name = result["name"]
    symbol = result["symbol"]
    price = result["price"]

    conn = sqlite3.connect(dbfile)
    db = conn.cursor()
    q = db.execute(f"SELECT cash FROM users WHERE username = '{username}'")
    total_cash = q.fetchone()[0]
    # if share already exists
    q2 = db.execute(f"SELECT share FROM shares WHERE symbol = '{symbol}' AND username='{username}'")
    row = q2.fetchone()

    if total_cash >= shares*price:
        money_left = round(total_cash - shares*price,2)
        with conn:
            if row:
                db.execute(f"UPDATE shares SET share={row[0]+shares}, price={price} WHERE symbol='{symbol}'")
            else:
                db.execute(f"INSERT INTO shares VALUES ('{username}','{symbol}','{name}',{shares},{price})")

            db.execute(f"UPDATE users SET cash={money_left} WHERE username='{username}'")
            # update history
            db.execute(f"INSERT INTO history VALUES ('{username}','{symbol}',{shares},{price},'{get_time()}')")
        flash(f"{shares} share of {name} Bought")
        return redirect("/")
    else:
        return apology("Can't Afford")

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    conn = sqlite3.connect(dbfile)
    db = conn.cursor()

    username = session["user_id"]
    q = db.execute(f"SELECT * FROM history WHERE username = '{username}'")
    rows = q.fetchall()
    return render_template("history.html", rows=rows)


@app.route("/login", methods=["GET", "POST"])
def login():

    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        conn = sqlite3.connect(dbfile)
        db = conn.cursor()

        username = request.form.get("username")
        password = request.form.get("password")

        # Ensure username was submitted
        if not username:
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not password:
            return apology("must provide password", 403)

        # Query database for username
        q = db.execute(f"SELECT * FROM users WHERE username = '{username}'")
        rows = q.fetchall()

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0][2], password):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0][1]

        # Redirect user to home page
        flash("Logged In")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")
    if request.method == "POST":
        symbol = request.form.get("symbol")
        result =  lookup(symbol)
        if not result:
            return apology("Invalid Symbol")
        n = result["name"]
        s = result["symbol"]
        p = result["price"]
        return render_template("quote_result.html", name=n, symbol=s, price=usd(p))

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        conn = sqlite3.connect(dbfile)
        db = conn.cursor()
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        q = db.execute(f"SELECT * FROM users WHERE username = '{username}'")
        rows = q.fetchall()
        if len(rows) == 0:
            hash = generate_password_hash(password)
            db.execute(f"INSERT INTO users (username,hash) VALUES ('{username}','{hash}')")
            session["user_id"] = username
            conn.commit()
            flash("Registered")
            return redirect("/")
        else:
            return apology("username already taken")
    return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    username = session["user_id"]
    conn = sqlite3.connect(dbfile)
    db = conn.cursor()

    if request.method == "GET":
        q = db.execute(f"SELECT symbol FROM shares WHERE username='{username}'")
        rows = q.fetchall()
        return render_template("sell.html", rows=rows)

    # else POST
    symbol = request.form.get("symbol")
    shares = int(request.form.get("shares"))
    result = lookup(symbol)
    price = result["price"]
    name = result["name"]

    q = db.execute(f"SELECT share FROM shares WHERE username='{username}' AND symbol='{symbol}'")
    total_shares = q.fetchone()[0]

    q = db.execute(f"SELECT cash FROM users WHERE username='{username}'")
    cash = q.fetchone()[0]

    total_cash = round(cash + shares*price, 2)
    share_left = total_shares -shares

    if shares > total_shares:
        return apology("Too Many Shares")

    elif shares == total_shares:
        with conn:
            db.execute(f"DELETE FROM shares WHERE username='{username}' AND symbol='{symbol}'")
    else:
        with conn:
            db.execute(f"UPDATE shares SET share={share_left} WHERE username='{username}' AND symbol='{symbol}'")

    with conn:
        db.execute(f"UPDATE users SET cash={total_cash} WHERE username='{username}'")
        # update history
        db.execute(f"INSERT INTO history VALUES ('{username}','{symbol}',{-shares},{price},'{get_time()}')")

    flash(f"Sold {shares} share of {name} Bought")
    return redirect("/")

@app.route("/profile", methods=["GET", "POST"])
@login_required
def profile():
    username = session["user_id"]
    # if request.method == "POST":

    return render_template("profile.html", username=username)

@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    """Change Password"""
    username = session["user_id"]
    if request.method == "POST":
        conn = sqlite3.connect(dbfile)
        db = conn.cursor()

        old_password = request.form.get("old_password")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Query database for username
        q = db.execute(f"SELECT * FROM users WHERE username = '{username}'")
        rows = q.fetchall()

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0][2], old_password):
            return apology("invalid password", 403)

        hash = generate_password_hash(password)
        with conn:
            q = db.execute(f"UPDATE users SET hash='{hash}' WHERE username = '{username}'")
            flash("Password changed")
            return redirect("/")
    # else get
    return render_template("change_password.html")

@app.route("/delete", methods=["GET", "POST"])
@login_required
def delete():
    """Delete Account"""
    username = session["user_id"]
    if request.method == "POST":
        conn = sqlite3.connect(dbfile)
        db = conn.cursor()

        password = request.form.get("password")

        # Query database for username
        q = db.execute(f"SELECT * FROM users WHERE username = '{username}'")
        rows = q.fetchall()

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0][2], password):
            return apology("invalid password", 403)

        with conn:
            q = db.execute(f"DELETE FROM users WHERE username='{username}'")
            q = db.execute(f"DELETE FROM history WHERE username='{username}'")
            q = db.execute(f"DELETE FROM shares WHERE username='{username}'")
        session.clear()
        flash("Account Deleted")
        return redirect("/")
    # else get
    return render_template("delete.html")

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)

# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)

if __name__ == "__main__":
    app.run()
