from flask import Flask
app = Flask(__name__)

import logging
# TODO: get service name through env
logging.basicConfig(filename='/var/log/service/app.log', encoding='utf-8', level=logging.DEBUG)

@app.route("/")
def hello():
    logging.info("Request")
    return "Flask API"

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=8080)
