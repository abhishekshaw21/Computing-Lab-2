# Name:mapper5.py
# Creator: Abhishek Shaw (abhishekshaw21@iitkgp.ac.in)
# Date: March 9,2020
# Description:Find all the items (item-ids) which cost more than 2 USD and have 
#			  at least 5 reviews with 5 star rating.​ ​(Hint: You will have to use both the 
#             input files. In <key,value> pairs, the key can also be a tuple too.)
import gzip
fileName1="Data/items.json.gz"
fileName2="Data/reviews.json.gz"
try:
	fp1=gzip.open(fileName1)
	fp2=gzip.open(fileName2)
except:
	print("File Does not exist")
	sys.exit()
for line in fp1:
	item=eval(line)
	try:
		if(float(item["price"])>2):
			print(f'{item["asin"]}\t0')
	except Exception as e:
		continue
for line in fp2:
	review=eval(line)
	try:
		if(review["overall"]==5):
			print(f'{review["asin"]}\t1')
	except Exception as e:
		pass