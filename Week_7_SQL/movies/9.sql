SELECT name FROM(
	SELECT DISTINCT people.id, people.name FROM people
	JOIN stars ON stars.person_id = people.id
	JOIN movies ON movies.id = stars.movie_id
	WHERE year=2004
	ORDER BY birth) ;
