# Name:mapper2.py
# Creator: Abhishek Shaw (abhishekshaw21@iitkgp.ac.in)
# Date: March 9,2020
# Description:Find the item (item-id) with the highest number of 4 or 5 stars ratings.
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
		rating=int(review['overall'])
		if(rating>=4):
			#print(rating,end=' ')
			print(f'{review["asin"]}\t1')
	except Exception as e:
		continue