from flask import Flask, request, jsonify
import os
import cv2
import numpy as np
import tensorflow as tf

app = Flask(__name__)

# 画像を保存するディレクトリ
UPLOAD_FOLDER = './'
os.makedirs(UPLOAD_FOLDER, exist_ok=True)

# TensorFlowモデルの読み込み
MODEL_PATH = 'mobilenet/saved_model'  # saved_modelのパス
model = tf.saved_model.load(MODEL_PATH)

def perform_inference(image):
    """
    受け取った画像に対して推論を行う関数
    """
    input_tensor = tf.convert_to_tensor(image)
    input_tensor = input_tensor[tf.newaxis, ...]  # バッチ次元を追加

    detections = model(input_tensor)

    # 検出結果を解析
    num_detections = int(detections.pop('num_detections'))
    detections = {key: value[0, :num_detections].numpy() for key, value in detections.items()}
    detections['num_detections'] = num_detections

    # クラスIDを整数に変換
    detections['detection_classes'] = detections['detection_classes'].astype(np.int64)

    return detections

@app.route('/upload', methods=['POST'])
def upload_image():
    # 画像ファイルを受信
    if 'file' not in request.files:
        return jsonify({"error": "No file part"}), 400

    file = request.files['file']

    if file.filename == '':
        return jsonify({"error": "No selected file"}), 400

    # 画像データを読み込み
    img_bytes = np.frombuffer(file.read(), np.uint8)
    image = cv2.imdecode(img_bytes, cv2.IMREAD_COLOR)

    if image is None:
        return jsonify({"error": "Failed to decode image"}), 400

    # TensorFlow Object Detectionで推論
    detections = perform_inference(image)

    # 検出結果を整理してクライアントに返す
    result = {
        "num_detections": int(detections['num_detections']),
        "detection_classes": detections['detection_classes'].tolist(),
        "detection_boxes": detections['detection_boxes'].tolist(),
        "detection_scores": detections['detection_scores'].tolist(),
    }

    # 仮に画像を保存する場合
    file_path = os.path.join(UPLOAD_FOLDER, file.filename)
    cv2.imwrite(file_path, image)

    return jsonify(result), 200

if __name__ == "__main__":
    # サーバーをホストして起動
    app.run(host='127.0.0.1', port=8881)
