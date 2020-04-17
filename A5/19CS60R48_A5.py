# Name:main.c
# Creator: Abhishek Shaw (abhishekshaw21@iitkgp.ac.in)
# Date: Februrary 9,2020
# Description:Web Crawler
import urllib.request, urllib.error, urllib.parse
import ply.lex as lex
import ply.yacc as yacc
import re

#Global Variables
author_name_temp=""
paper_year_temp=2020
paper_name_temp=""
author_name=[]
paper_type=[]
paper_year=[]
paper_name=[]
paper_venue=[]

#functions for lexical analyzer starts here
tokens = (
	'S_TITLE',
	'E_TITLE',
	'S_PTYPE',
	'E_PTYPE',
	'S_PNAME',
	'E_PNAME',
	'S_PYEAR',
	'E_PYEAR',
	'VENUE',
	'ALL',
)

def t_S_PTYPE(t):
	r'<div\sclass="nr"\sid="[a-z][0-9]+">\['
	return t

def t_E_PTYPE(t):
	r']</div>'
	return t

def t_S_PNAME(t):
	r'<span\sclass="title"\sitemprop="name">'
	return t

def t_E_PNAME(t):
	r'</span>'
	return t

def t_S_PYEAR(t):
	r'<li\sclass="year">'
	return t

def t_E_PYEAR(t):
	r'</li>'
	return t

def t_S_TITLE(t):              
	r'<title>'
	return t

def t_E_TITLE(t):              
	r'</title>'
	return t

def t_ALL(t):
	r'[a-z|A-Z|0-9|\^\*\(\)\.\+\-\\,:\s!@#$%&*]+'
	return t

def t_VENUE(t):
	r'<span\sitemprop=\"isPartOf\"[^<]+<span\sitemprop=\"name\">([^<]+)'
	pattern = re.compile(r'<span\sitemprop=\"isPartOf\"[^<]+<span\sitemprop=\"name\">([^<]+)')
	t.value = pattern.sub(r'\1',t.value)
	return t

def t_error(t):
	t.lexer.skip(1)

#functions for lexical analyzer ends here

#fucntions for parser starts here
def p_author(p):
	'Start  : S_TITLE ALL E_TITLE'
	global author_name_temp
	author_name_temp = str(p[2][6:])

def p_type(p):
	'Start : S_PTYPE ALL E_PTYPE'
	global paper_type_temp
	paper_type_temp=str(p[2])
	

def p_year(p):
	'Start : S_PYEAR ALL E_PYEAR'
	global paper_year_temp
	paper_year_temp = int(p[2])

def p_pname(p):
	'Start : S_PNAME ALL E_PNAME'
	global paper_name_temp
	paper_name_temp=str(p[2])

def p_venue(p):
	'Start : VENUE'
	global paper_venue
	global paper_name
	global author_name
	global paper_year
	global paper_type
	global author_name_temp
	global paper_year_temp
	global paper_type_temp
	global paper_name_temp
	paper_venue.append(str(p[1]))	
	paper_name.append(paper_name_temp)
	author_name.append(author_name_temp)
	paper_year.append(paper_year_temp)
	paper_type.append(paper_type_temp)

def p_error(p):
	pass

#functions for parser ends here
def downloadHtml(htmlFileNames):
	inputFileName="professors.txt"
	with open(inputFileName) as f:
		content = f.readlines()
	content = [x.strip() for x in content]
	f = open('professorFileName.txt', 'w')
	f.close() 
	for url in content:
		try:
			print(f'Fetching web page: {url}')
			response = urllib.request.urlopen(url)
			webContent = response.read()
			name=url[url.rindex('/')+1:]
			name=name.replace(':','_')
			htmlFileNames.append(name)
			with open('professorFileName.txt','a') as fp:
				fp.write(name+'\n')
			f = open(name, 'wb')
			f.write(webContent)
			f.close()
		except Exception as e:
			print(f'url: {url} skipped,\n Reason: {e}')

def populateName(htmlFileNames):
	if not htmlFileNames:
		with open('professorFileName.txt') as f:
			nameTemp = f.readlines()
		nameTemp = [x.strip() for x in nameTemp]
		for x in nameTemp:
			htmlFileNames.append(x)	

def main():
	htmlFileNames=[]
	downloadHtml(htmlFileNames)
	populateName(htmlFileNames)
	for prof in htmlFileNames:
		with open(prof, 'r') as myfile:
			data = myfile.read()
			lexer = lex.lex()
			parser = yacc.yacc()
			parser.parse(data)
	print("All entries")
	for i in range(len(author_name)):
	 	print(f'{author_name[i]} | {paper_type[i]} | {paper_year[i]} | {paper_name[i]} | {paper_venue[i]}')
	while True:
		flag=0
		print("*************************************************************")
		selectedrow=[0]*len(author_name)
		print("Enter the filteration criteria(press enter to skip the field)")
		print("1.Name",end=' ')
		name1=input()
		name1=name1.strip()
		print("2.Title",end=' ')
		title2=input()
		title2=title2.strip()
		print("3.Type of publication",end=' ')
		typeOfPublication3=input()
		typeOfPublication3=typeOfPublication3.strip()
		print("4.Year",end=' ')
		year4=input()
		year4=year4.strip()
		print("5.Venue ",end=' ')
		venue5=input()
		if len(typeOfPublication3)>1:
			print("Invalid Type of publication")
			continue
		#Name
		if(len(name1)==0):
			selectedrow=[inc+1 for inc in selectedrow]
		else:
			for i in range(len(author_name)):
				if(name1.casefold() in author_name[i].casefold()):
					selectedrow[i]=selectedrow[i]+1
		#Title
		if(len(title2)==0):
			selectedrow=[inc+1 for inc in selectedrow]
		else:
			for i in range(len(paper_name)):
				if(title2.casefold() in paper_name[i].casefold()):
					selectedrow[i]=selectedrow[i]+1
		#TypeOfPublication
		if(len(typeOfPublication3)==0):
			selectedrow=[inc+1 for inc in selectedrow]
		else:
			for i in range(len(author_name)):
				if(typeOfPublication3[0].casefold()==paper_type[i][0].casefold()):
					selectedrow[i]=selectedrow[i]+1
		#Year
		if(len(year4)==0):
			selectedrow=[inc+1 for inc in selectedrow]
		else:
			try:
				year4=int(year4)
			except Exception as e:
				print("Invalid Year entered")
				continue
			for i in range(len(paper_year)):
				if(year4==paper_year[i]):
					selectedrow[i]=selectedrow[i]+1
		#Venue
		if(len(venue5)==0):
			selectedrow=[inc+1 for inc in selectedrow]
		else:
			for i in range(len(author_name)):
				if (venue5.casefold() in paper_venue[i].casefold()):
					selectedrow[i]=selectedrow[i]+1
		for i in range(len(selectedrow)):
			if selectedrow[i]==5:
				print(f'Author Name:{author_name[i]}')
				print(f'Paper Type:{paper_type[i]}')
				print(f'Paper Year:{paper_year[i]}')
				print(f'Paper Name:{paper_name[i]}')
				print(f'Venue:{paper_venue[i]}')
				print("-------------------------------------------------------------")
				flag=1
		if len(selectedrow)==0 or flag==0:
			print("No match found")
		x=input("Enter 1 to continue (any other key to exit) ")
		if x != "1":
			break
if __name__ == "__main__":
	main()