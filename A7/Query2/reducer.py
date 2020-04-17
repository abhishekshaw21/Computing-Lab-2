# Name:reducer2.py
# Creator: Abhishek Shaw (abhishekshaw21@iitkgp.ac.in)
# Date: March 9,2020
# Description:Find the item (item-id) with the highest number of 4 or 5 stars ratings.
import sys

current_word = None
current_count = 0
word = None
max_word=None
max_count=0
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
		if current_word and current_count>max_count:
			max_word=current_word
			max_count=current_count
		current_count = count
		current_word = word
if current_word == word and current_count>max_count:
	max_word=current_word
	max_count=current_count
print(max_word)