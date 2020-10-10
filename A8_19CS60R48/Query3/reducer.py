# Name:reducer3.py
# Creator: Abhishek Shaw (abhishekshaw21@iitkgp.ac.in)
# Date: March 9,2020
# Description:Find all the users (user-ids) who have rated at least 10 products.
import sys

current_word = None
current_count = 0
word = None
flag=0
for line in sys.stdin:
	line = line.strip()
	# parse the input we got from mapper.py
	word, count = line.split('\t', 1)
	try:
		count = int(count)
	except ValueError:
		continue
	if current_word == word:
		current_count += count
	else:
		if current_word and current_count>=5:
			if(flag==1):
			   print (current_word)
		if(count==0):
			flag=1
		else:
			flag=0
		current_count = count
		current_word = word
if current_word == word and current_count>=5:
	if flag==1:
		print (current_word)