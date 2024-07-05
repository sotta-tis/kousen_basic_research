import socket
import json

def start_server(host='127.0.0.1', port=8893):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((host, port))
        s.listen()
        print(f"Server listening on {host}:{port}")

        while True:
            conn, addr = s.accept()
            with conn:

                print(f"Connected by {addr}")
                while True:
                    data = conn.recv(1024)
                    if not data:
                        break
                    command = data.decode('UTF-8')
                    print(f"Received command: {command}")

                    # Echo back a generic success response
                    response = json.dumps({"is_sccess": True, "msg": "Received"}).encode('UTF-8')
                    conn.sendall(response)

if __name__ == "__main__":
    start_server()
