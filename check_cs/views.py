import shutil
from flask_sqlalchemy import SQLAlchemy
from werkzeug.utils import secure_filename
import os
import glob
from flask import Flask
from flask import request,send_from_directory,send_file
from zipfile import ZipFile
from shutil import copyfile
import datetime

app=Flask(__name__)
import io
import time

@app.route('/', methods=['POST', 'GET'])
def home():
    """
    A view for the default home route.
    Accepts GET and POST methods
    :return: A string
    """
    g.processing_time_data = ""
    return '<h1> Welcome Here </h1>'
'''

app1 = app
app1.config['SQLALCHEMY_DATABASE_URI'] = '*************************************************'
db = SQLAlchemy(app1)
#db.create_all()
class Abc(db.Model):
    __tablename__ ='done'
    __table_args__={
        'autoload':True,
        'autoload_with': db.engine
        }
    db.init_app(app1)

    def __repr__(self):
        return "<abc(file_name={},time_stamp={},delete_flag={})>".format(self.file_name,self.time_stamp,self.delete_flag)
'''



#
# @app.before_request
# def before_request():
#     g.request_start_time = time.time()
#     if not Path(app.config['PROCESSING_TIME_CSV_FILE']).is_file():
#         with open(app.config['PROCESSING_TIME_CSV_FILE'], 'a') as file:
#             file.write(
#                 "API, File Received, File Type, Time Taken (Save Image), Time Taken (Tesseract), Time taken (Save Text File), Total Time Taken\n")
#
#
# @app.after_request
# def after_request(response):
#     total_time_taken = time.time() - g.request_start_time
#     if ((response.response) and (200 <= response.status_code < 300) and (
#             response.content_type.startswith('text/html'))):
#         # data to the file should be written just once to overcome write race conditions
#         with open(app.config['PROCESSING_TIME_CSV_FILE'], 'a') as file:
#             file.write(g.processing_time_data + str(total_time_taken) + "\n")
#     return response

# TODO check users against the registered API keys
@app.route('/api/v1/file_trans', methods=['POST','GET'])
def image_to_text():
    #ap=db.session.query(abc.filename)
    #print(ap)
    """
    Handles the API route of file_trans.
    Accepts 'POST' method.
    Checks for Authorization header present in request and validates it against the assigned key in registered_api_keys db.
    The Authorization header contains the Api key assigned to the user.
    :return: Text extracted from image or the respective error code
            """

    submittedfile=request.files['file']
    filename=secure_filename(submittedfile.filename)
    path="/home/ubuntu" 
    out={'filename':submittedfile.filename}
    submittedfile.save(os.path.join(path,filename))
    
    with ZipFile(filename, 'r') as zip:  
        zip.extractall() 
 
    os.system("cp /home/ubuntu/file_to_send/* /home/ubuntu/code/")
    os.remove("/home/ubuntu/file_to_send.zip")
   # os.remove("-r /home/ubuntu/file_to_send")
    shutil.rmtree("/home/ubuntu/file_to_send", ignore_errors=False, onerror=None)


    return send_from_directory("/home/ubuntu", filename="user_info.txt",as_attachment=True)

@app.route('/api/v1/data_server', methods=['POST','GET'])
def data_server_client():
    print("VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV")
    #submittedfile=request.files['file']
    #filename=secure_filename(submittedfile.filename)
    #path="/home/ubuntu" 
    #out={'filename':submittedfile.filename}
    #submittedfile.save(os.path.join(path,filename))
    #os.system("zip -r check_cs.zip /home/ubuntu/check_cs")
    shutil.make_archive("check_cs", 'zip', "/home/ubuntu/check_cs")
    #return send_from_directory("/home/ubuntu", filename="check_cs.zip",as_attachment=True)
    return send_file("/home/ubuntu/check_cs.zip",mimetype='application/zip',as_attachment=True,attachment_filename="check_cs.zip")


@app.route('/api/v1/update', methods=['POST','GET'])
def update_client():    
    #os.remove("/home/ubuntu/update_folder.zip")
    submittedfile=request.files['file']
    filename=secure_filename(submittedfile.filename)
    path="/home/ubuntu" 
    out={'filename':submittedfile.filename}
    submittedfile.save(os.path.join(path,filename))
    
    testsite_array = []
    testsite_array2 = []
    with open('server_version.txt') as my_file:
        for line in my_file:
            uu1=line.split('-')
    with open('client_version.txt') as my_file:
        for line in my_file:
            uu2=line.split('-')
    d1=int(uu1[1])
    d2=int(uu2[1])
    file1 = open("update_info.txt","w") 
    if d1>d2:
        file1.write("update is sent to your device")
    if d1<d2:
        file1.write("update is Not required to your device")
    file1.close()
    os.system("mkdir update_folder")
    os.system("cp /home/ubuntu/update_info.txt /home/ubuntu/update_folder")
    if d1>d2:
        os.system("cp /home/ubuntu/server_version.txt /home/ubuntu/update_folder")
        os.system("cp -r /home/ubuntu/check_cs /home/ubuntu/update_folder")
    shutil.make_archive("update_folder", 'zip', "/home/ubuntu/update_folder")
    os.remove("/home/ubuntu/update_info.txt")
    shutil.rmtree("/home/ubuntu/update_folder", ignore_errors=False, onerror=None)
    
    return send_from_directory("/home/ubuntu", filename="update_folder.zip",as_attachment=True)
   



if __name__ == "__main__":
    """ host: defines the host IP. Should be 0.0.0.0 for running a server that can be accessible from a website
        port: define the port on which the server accpets requests
        ssl_context: a tuple of SSL sertificate and certificate key. Generated using openssl
    """
    app.run( port=5000, host = "0.0.0.0" , ssl_context=('certificate.pem', 'certificate_key.pem'))
