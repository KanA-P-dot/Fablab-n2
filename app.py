from flask import Flask, request, jsonify
from datetime import datetime

app = Flask(__name__)
app.static_folder = 'static'
avis_list = []

@app.route('/post-json')
def post_json():
    response1 = request.args.get("response1")
    response2 = request.args.get("response2")
    avis_list.append({'date': datetime.now().strftime("%Y-%m-%d %H:%M:%S"), 'prof': response1, 'course': response2})
    return jsonify({'status': 'success'}), 200

@app.route('/')
def index():
    return '''
        <!DOCTYPE html>
        <html lang="en">
        <head>
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <title>Feedback</title>
            <!-- Bootstrap CSS -->
            <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css">
            <style>
                body {{
                    padding: 20px;
                }}
                h1 {{
                    margin-bottom: 20px;
                    text-align: center;
                }}
                table {{
                    width: 100%;
                    margin-top: 20px;
                    border-collapse: separate;
                }}
                th, td {{
                    padding: 10px;
                    text-align: left;
                    border-top:none;
                }}
                th {{
                    background-color: #f2f2f2;
                    border-radius: 25px;

                }}
                tr:nth-child(even) {{
                    background-color: #f9f9f9;
                }}
                .logo {{
                    width: 500px;
                    margin-top:-75px;
                    position: relative;
                    left: 50%;
                    transform: translateX(-50%);
                }}
            </style>
        </head>
        <body>
            <div class="container">
                <img src="/static/logo.png" alt="Logo" class="logo">
                <table class="table" cellspacing="0" cellpadding="0">
                    <thead class="thead-light">
                        <tr>
                            <th>Date</th>
                            <th>Avis Prof</th>
                            <th>Avis Cours</th>
                        </tr>
                    </thead>
                    <tbody>
                        {rows}
                    </tbody>
                </table>
            </div>
        </body>
        </html>
    '''.format(rows=''.join(f'<tr><td>{item["date"]}</td><td>{item["prof"]}</td><td>{item["course"]}</td></tr>' for item in avis_list))

if __name__ == '__main__':
    app.run(debug=True, host="0.0.0.0")
