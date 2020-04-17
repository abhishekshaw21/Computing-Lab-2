# Name:main.c
# Creator: Abhishek Shaw (abhishekshaw21@iitkgp.ac.in)
# Date: Februrary 10,2020
# Description:Web Crawler
import urllib.request, urllib.error, urllib.parse
import ply.lex as lex
import ply.yacc as yacc

conferenceList=[]

tokens = (
	'STR',
	'ETR',
	'ALL',
)

def t_STR(t):
	r'<a\shref="http:\/\/dblp\.org\/db\/conf\/[a-z|\/|\.]*">'
	return t

def t_ETR(t):
	r'</a>'
	return t

def t_ALL(t):
	r'[a-z|A-Z|0-9|\^\*\(\)\.\+\-\\,:\s!@#$%&*]+'
	return t

def t_error(t):
	t.lexer.skip(1)

def p_conferenceName(p):
	'Start  : STR ALL ETR'
	global conferenceList
	conferenceList.append(str(p[2]))

def p_error(p):
	pass

def main(): 
	url="http://csrankings.org/"
	try:
		print(f'Fetching web page: {url}')
		response = urllib.request.urlopen(url)
		webContent = response.read()
		name="csrankings"
		f = open(name, 'wb')
		f.write(webContent)
		f.close()
	except Exception as e:
		print(f'url: {url} skipped,\n Reason: {e}')
	with open(name, 'r') as myfile:
		data = myfile.read()
		lexer = lex.lex()
		parser = yacc.yacc()
		parser.parse(data)
	with open('conferenceList.txt','w') as fp:
		for venue in conferenceList:
			fp.write(venue+'\n')
	print(f'{len(conferenceList)} venues successfully written in conferenceList.txt')

if __name__ == '__main__':
	main()