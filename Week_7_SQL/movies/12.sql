SELECT movies.title FROM people
JOIN stars ON stars.person_id = people.id
JOIN movies ON movies.id = stars.movie_id
WHERE name = 'Johnny Depp'
AND movie_id IN (
	SELECT movie_id FROM people
	JOIN stars on stars.person_id = people.id
	where name = 'Helena Bonham Carter'
);
