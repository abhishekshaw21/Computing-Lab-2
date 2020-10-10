# Name:mapper4.py
# Creator: Abhishek Shaw (abhishekshaw21@iitkgp.ac.in)
# Date: March 9,2020
# Description:Find top-10 users (user-ids) who have the highest number of reviews with length more than 20 characters.
import gzip
fileName2="Data/reviews.json.gz"
try:
	fp=gzip.open(fileName2)
except:
	print("File Does not exist")
	sys.exit()
for line in fp:
	review=eval(line)
	try:
		reviewLength=len(review["reviewText"])
		if(reviewLength>20):
			print(f'{review["reviewerID"]}\t1')
	except Exception as e:
		continue