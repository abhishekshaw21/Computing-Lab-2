# Name:mapper1.py
# Creator: Abhishek Shaw (abhishekshaw21@iitkgp.ac.in)
# Date: March 9,2020
# Description:Find all the users (user-ids) who have rated at least 10 products.
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
		print(f'{review["reviewerID"]}\t1')
	except Exception as e:
		continue