from time import gmtime, strftime, localtime,sleep
import socket
import threading 

def update_html(current_temp,current_time,status):
	
	string1 = """<!DOCTYPE html>
	<html>
	<head>
	<title>Temperature monitor</title>
	</head>
	<body>
	
	<h1>Current temperature is """
	string4 = """.</p><p></p>
	<p>Previous recorded temperature : </p>

	<div>
	<object data="data.txt" type="text/plain"
	width="100%" style="height: 50em">
	<a href="data.txt">Embedded Text Document</a>
	</object>
	</div>


	</body>
	</html> """
	string2 = """ degree Celcius.  """

	string3 = """</h1>
	<p></p>
	<p>Curret time is """

	
	with open('index.html','w') as html: 
		html.write(string1)
		html.write(str(current_temp))
		html.write(string2)
		html.write(str(status))
		html.write(string3)
		html.write(str(current_time))
		html.write(string4)
		
def update_text(current_temp,current_time,status):
	with open('data.txt','r') as text:
		temp = text.read()
	with open('data.txt','w') as text:
		text.write(str(current_time))
		text.write("\t")
		text.write(str(current_temp))
		text.write(" C\t")
		text.write(str(status))
		text.write("\n")
		text.write(temp)
		
def ask_temp():
	sensor_ip = '192.168.1.146'
	sensor_port= 8713
	buffer_size= 1024
	socket.setdefaulttimeout(30)  #timeout in seconds
	flag = 1
	
	while flag ==1:
		with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
			try:
				data = s.connect((sensor_ip, sensor_port))
				print("conn_openning")
			except OSError : pass
			except socket.error : pass
			except socket.timeout : pass
			
			try:
				data = s.recv(buffer_size)
				print("finish_wait")
			except OSError : pass
			except socket.error : pass
			except socket.timeout : pass
			
			s.close()
		
		if type(data) == bytes:
			d_data=data.decode("ASCII")
			
			d_len = len(d_data)

			temperature = d_data[7:9]
			status = d_data[26:d_len]
			print("data_get")

		else :
			temperature = "NA"
			status = "NA"
		
		with doc_lock:
			with open('buf.txt','w') as buf:
				buf.write(temperature)
			with open('buf1.txt','w') as buf1:
				buf1.write(status)
				
		sleep(0.5)

def main():
		
		
	temp = threading.Timer(1,ask_temp)
	
	temp.start()	
	flog =1
	
	while flog ==1:
		
		with doc_lock:
			with open('buf.txt','r') as buf:
				current_temp=buf.read()
			with open('buf1.txt','r') as buf1:
				status=buf1.read()

		current_time = strftime("%a, %d %b %Y %H:%M",localtime())
		update_html(current_temp,current_time,status)
		update_text(current_temp,current_time,status)
		sleep(60)

doc_lock=threading.RLock()
main()
	
	
	
