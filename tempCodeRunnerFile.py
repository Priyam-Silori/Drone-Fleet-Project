from flask import Flask, send_from_directory

app = Flask(__name__, static_folder="frontend/home", static_url_path="")

# Serve index.html by default
@app.route("/")
def serve_home():
    return send_from_directory(app.static_folder, "index.html")

# Serve any other page (like admindashboard.html, login.html, etc.)
@app.route("/<path:path>")
def serve_page(path):
    return send_from_directory(app.static_folder, path)

if __name__ == "__main__":
    app.run(debug=True, port=5000)
