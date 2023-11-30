import http.client

# Your chunked data
text = "This is a longer text that will be split into multiple chunks for a chunked transfer encoding."
chunked_data = []
chunk_size = 10  # Split text into chunks of size 10 characters

for i in range(0, len(text), chunk_size):
    chunk = text[i:i + chunk_size]
    chunked_data.append(f"{len(chunk):X}\r\n{chunk}\r\n".encode())

chunked_data.append(b'0\r\n\r\n')  # Add the final chunk indicating the end

# Set up the connection
conn = http.client.HTTPConnection('127.0.0.1:8080')
headers = {
    'Transfer-Encoding': 'chunked',
    'Content-Type': 'text/plain'
}

try:
    # Send the request
    conn.request('POST', '/test.txt', headers=headers)

    # Send chunked data
    for chunk in chunked_data:
        conn.send(chunk)

    # Get the response
    response = conn.getresponse()

    if response.status >= 200 & response.status < 300:
        print("Request successful. Response:")
        print(response.read().decode('utf-8'))
    else:
        print("Request failed. Status code:", response.status)

except http.client.HTTPException as e:
    print("An error occurred:", e)

finally:
    conn.close()
