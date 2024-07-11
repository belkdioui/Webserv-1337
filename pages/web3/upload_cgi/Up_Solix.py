# import os
# import re
# import sys
# import uuid


# def parse_filename(content):
#     filename_pattern = r'filename="([^"]+)"'
#     match = re.search(filename_pattern, content, re.IGNORECASE)
#     return match.group(1).strip() if match else ""


# def handle_upload(request_body, upload_dir):
#     boundary = os.environ.get("CONTENT_TYPE", "").split("boundary=")[-1]
    
#     if not boundary:
#         return "<h1>Error: Could not find boundary in Content-Type</h1>"

#     parts = request_body.split(boundary.encode())[1:-1]

#     for part in parts:
#         header, data = part.split(b'\r\n\r\n', 1)
#         print("IM HEREEEEEEEE", file=sys.stderr)
#         filename = parse_filename(header.decode())
#         if not filename:
#             continue
    
#         filename, file_extension = os.path.splitext(filename)
#         filename = f"{filename}-{str(uuid.uuid4())}{file_extension}"
#         file_path = os.path.join(upload_dir,filename)

#         try:
#             with open(file_path, "wb") as f:
#                 data = data.split(b'\r\n--', 1)[0]
#                 f.write(data)
#             return f"<h1>201 - Created</h1><h3>{filename} has been uploaded successfully!</h3><h3>Go to upload directory to see your file</h3>"
#         except Exception as e:
#             return f"<h1>Error during upload: {str(e)}</h1>"

#     return "<h1>No file found in upload data</h1>"



# def main():
#     method = os.environ.get("REQUEST_METHOD", "")
#     if method != "POST":
#         exit(1)

#     content_length = os.environ.get("CONTENT_LENGTH", "")
#     if not content_length:
#         print("HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 22\r\nConnection: keep-alive\r\n\r\nError: Invalid request")
#         exit(0)

#     upload_dir = os.environ.get("UPLOAD_DIR", "")
#     upload_dir = "./"
#     if not upload_dir:
#         print("HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/plain\r\nContent-Length: 31\r\nConnection: keep-alive\r\n\r\nError: Upload directory not set")
#         exit(0)

#     # Read the entire request body
#     try:
#         request_body = sys.stdin.buffer.read()
#     except Exception as e:
#         exit(1)

#     response_body = "<!DOCTYPE html><html><head><title>Upload</title><meta charset=\"UTF-8\" />"
#     response_body += "<style>body { font-family: sans-serif; background-color: #141615; color: #3a87ff; font-size: 2em; text-align: center; }</style>"
#     response_body += "</head><body>"
#     data = handle_upload(request_body, upload_dir)
#     if data[:7] == "<h1>201":
#         status = "201 Created"
#     else:
#         status = "400 Bad Request"
#     response_body += data
#     response_body += "</body></html>"

#     print(f"HTTP/1.1 {status}\r\n", end="")
#     print("Content-Type: text/html\r\n", end="")
#     print("Content-Length: " + str(len(response_body)) + "\r\n", end="")
#     print("Connection: keep-alive\r\n", end="")
#     print("\r\n", end="")
#     print(response_body)

# if __name__ == "__main__":
#     main()


#!/usr/bin/python3

import cgi
import cgitb
import os
import sys
cgitb.enable()

# Current working directory
form = cgi.FieldStorage()

def upload(filename):
    if filename not in form:
        print("Failed to upload file!", file=sys.stderr)
        exit()

    fileitem = form[filename]

    # Check if file was uploaded
    if fileitem.filename:
        # Strip leading path from file name to avoid directory traversal attacks
        filename = os.path.basename(fileitem.filename)
        upload_dir = './'  # Make sure this directory exists and has write permissions
        
        # Clean filename
        clean_filename = ''.join(e for e in filename if e.isalnum() or e in ['.', '-']).rstrip()
        file_path = os.path.join(upload_dir, clean_filename)

        # Avoid overwriting existing files
        counter = 1
        while os.path.exists(file_path):
            filename, file_extension = os.path.splitext(clean_filename)
            file_path = os.path.join(upload_dir, f"{filename}_{counter}{file_extension}")
            counter += 1

        # Retrieve file data and save
        file_data = fileitem.file.read()
        with open(file_path, 'xb') as f:
            f.write(file_data)
        response_body = "<!DOCTYPE html><html><head><title>Upload</title><meta charset=\"UTF-8\" />"
        response_body += "<style>body { font-family: sans-serif; background-color: #141615; color: #3a87ff; font-size: 2em; text-align: center; }</style>"
        response_body += "</head><body>"
        response_body += f"<h1>201 - Created</h1><h3>{filename} has been uploaded successfully!</h3><h3>Go to upload directory to see your file</h3>"
        response_body += "</body></html>"

        print(f"HTTP/1.1 201 \r\n", end="")
        print("Content-Type: text/html\r\n", end="")
        print("Content-Length: " + str(len(response_body)) + "\r\n", end="")
        print("Connection: keep-alive\r\n", end="")
        print("\r\n", end="")
        print(response_body)

        # print("File '" + filename + "' uploaded and saved as '" + os.path.basename(file_path) + "'!")
    else:
        print("Failed to upload file!")


upload('file1')
# upload('file2')
# upload('file3')