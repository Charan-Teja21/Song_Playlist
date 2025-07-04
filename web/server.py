from flask import Flask, request, jsonify, render_template, send_from_directory
import os
import subprocess
import threading
from werkzeug.utils import secure_filename

UPLOAD_FOLDER = 'uploads'
ALLOWED_EXTENSIONS = {'mp3', 'wav', 'ogg', 'm4a'}

app = Flask(__name__)
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER

# Ensure uploads directory exists
os.makedirs(UPLOAD_FOLDER, exist_ok=True)

# Start the C backend as a subprocess
backend = subprocess.Popen(['../playlist'], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, bufsize=1)
lock = threading.Lock()

def send_command(cmd):
    with lock:
        backend.stdin.write(cmd + '\n')
        backend.stdin.flush()
        resp = backend.stdout.readline().strip()
        return resp

@app.route('/')
def home():
    return render_template('index.html')

@app.route('/list')
def list_songs():
    resp = send_command('LIST')
    parts = resp.split('|')
    if parts[0] != 'SONGS':
        return jsonify({'error': resp}), 400
    count = int(parts[1])
    songs = []
    for i in range(count):
        base = 2 + i * 5
        songs.append({
            'index': int(parts[base]),
            'title': parts[base+1],
            'artist': parts[base+2],
            'duration': int(parts[base+3]),
            'filepath': parts[base+4]
        })
    return jsonify({'songs': songs})

@app.route('/add', methods=['POST'])
def add_song():
    data = request.json
    cmd = f"ADD|{data['title']}|{data['artist']}|{data['duration']}|{data['filepath']}"
    resp = send_command(cmd)
    return jsonify({'result': resp})

@app.route('/delete', methods=['POST'])
def delete_song():
    data = request.json
    cmd = f"DELETE|{data['title']}"
    resp = send_command(cmd)
    return jsonify({'result': resp})

@app.route('/shuffle', methods=['POST'])
def shuffle():
    resp = send_command('SHUFFLE')
    return jsonify({'result': resp})

@app.route('/sort_title', methods=['POST'])
def sort_title():
    resp = send_command('SORT_TITLE')
    return jsonify({'result': resp})

@app.route('/sort_artist', methods=['POST'])
def sort_artist():
    resp = send_command('SORT_ARTIST')
    return jsonify({'result': resp})

@app.route('/upload', methods=['POST'])
def upload_file():
    if 'file' not in request.files:
        return jsonify({'error': 'No file part'}), 400
    file = request.files['file']
    if file.filename == '':
        return jsonify({'error': 'No selected file'}), 400
    if '.' in file.filename and file.filename.rsplit('.', 1)[1].lower() in ALLOWED_EXTENSIONS:
        filename = secure_filename(file.filename)
        save_path = os.path.join(app.config['UPLOAD_FOLDER'], filename)
        file.save(save_path)
        return jsonify({'filepath': save_path})
    return jsonify({'error': 'Invalid file type'}), 400

@app.route('/uploads/<filename>')
def uploaded_file(filename):
    return send_from_directory(app.config['UPLOAD_FOLDER'], filename)

if __name__ == '__main__':
    app.run(debug=True) 