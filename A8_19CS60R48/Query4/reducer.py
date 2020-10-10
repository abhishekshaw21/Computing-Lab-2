# Name:reducer4.py
# Creator: Abhishek Shaw (abhishekshaw21@iitkgp.ac.in)
# Date: March 9,2020
# Description:Find top-10 users (user-ids) who have the highest number of reviews with length more than 20 characters.
import sys
from operator import itemgetter

current_word = None
current_count = 0
word = None
reviewIdList=[]
minId=1000
countSize=0
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
		if current_word:
			t=[current_word,current_count]
			if countSize<10:
				reviewIdList.append(t)
				countSize=countSize+1
				reviewIdList=sorted(reviewIdList,key=itemgetter(1))
				minId=min(minId,reviewIdList[0][1])
			elif current_count>minId:
				reviewIdList[0][0]=current_word
				reviewIdList[0][1]=current_count
				reviewIdList=sorted(reviewIdList,key=itemgetter(1))
				minId=reviewIdList[0][1]
		current_count = count
		current_word = word
if current_word == word:
	t=[current_word,current_count]
	if current_count>minId:
		reviewIdList[0][0]=current_word
		reviewIdList[0][1]=current_count
		reviewIdList=sorted(reviewIdList,key=itemgetter(1))
		minId=reviewIdList[0][1]
reviewIdList=sorted(reviewIdList,key=itemgetter(1))
for id1 in reversed(reviewIdList):
	print(id1[0])