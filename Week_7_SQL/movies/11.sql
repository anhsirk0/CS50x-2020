SELECT movies.title FROM people
JOIN stars ON stars.person_id = people.id
JOIN ratings ON ratings.movie_id = stars.movie_id
JOIN movies on movies.id = ratings.movie_id
WHERE people.name = 'Chadwick Boseman'
ORDER BY rating DESC LIMIT 5;
