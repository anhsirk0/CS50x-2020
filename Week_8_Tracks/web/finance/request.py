import os , requests
symbol = 'aapl'
def lookup():
    api_key = os.environ.get("API_KEY")
    response = requests.get(f"https://cloud-sse.iexapis.com/stable/stock/{symbol}/quote?token={api_key}")
    response.raise_for_status()
    print(response.json())

lookup()
