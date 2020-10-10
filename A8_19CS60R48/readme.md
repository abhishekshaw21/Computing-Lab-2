Requirement:Python(3.6 or above)
Make sure the files are present in Data folder
--------------------------------------------------------------------------------
Running All Queries using makefile:
command: $ make run
--------------------------------------------------------------------------------
The mapper and reducer are used to get the results of the following queries.
There are 5 queries and command to run each is listed below, each query result is stored in corresponding results.txt for the query, to run them seperately.
--------------------------------------------------------------------------------
1. Find all the users (user-ids) who have rated at least 10 products. ​
$ python3 Query1/mapper.py |sort|python3 Query1/reducer.py >Query1/result.txt
--------------------------------------------------------------------------------
2. Find the item (item-id) with the highest number of 4 or 5 stars ratings.
$ python3 Query2/mapper.py |sort|python3 Query2/reducer.py >Query2/result.txt
--------------------------------------------------------------------------------
3. Find all the items (item-ids) which cost more than 3 USD and have appeared in “related”-->”also_viewed” section of at least 5 of the items. 
$ python3 Query3/mapper.py |sort|python3 Query3/reducer.py >Query3/result.txt
--------------------------------------------------------------------------------
4. Find top-10 users (user-ids) who have the highest number of reviews with length more
than 20 characters. ​
$ python3 Query4/mapper.py |sort|python3 Query4/reducer.py >Query4/result.txt
--------------------------------------------------------------------------------
5. Find all the items (item-ids) which cost more than 2 USD and have at least 5 reviews with 5 star rating.
$ python3 Query5/mapper.py |sort|python3 Query5/reducer.py >Query5/result.txt
