# Name:mapper3.py
# Creator: Abhishek Shaw (abhishekshaw21@iitkgp.ac.in)
# Date: March 9,2020
# Description:Find all the items (item-ids) which cost more than 3 USD and have appeared in
#			  “related”-->”also_viewed” section of at least 5 of the items.import gzip
import gzip
fileName2="Data/items.json.gz"
try:
	fp=gzip.open(fileName2)
except:
	print("File Does not exist")
	sys.exit()
for line in fp:
	item=eval(line)
	try:
		#print(item["price"])
		try:
			cost=float(item["price"])
			if(cost>3.0):
				print(f'{item["asin"]}\t0')
		except Exception as e:
			pass
		for relatedItem in item["related"]["also_viewed"]:
			print(f'{relatedItem}\t1')
			pass
	except Exception as e:
		pass