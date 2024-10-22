import cv2
import numpy as np
import tensorflow as tf
import re
import time  # FPS計測のためのtimeモジュールをインポート

# ラベルマップを読み込む関数
def load_label_map(label_map_path):
    category_index = {}
    with open(label_map_path, 'r') as f:
        for line in f:
            if 'id' in line:
                class_id = int(re.search(r'\d+', line).group())
            if 'name' in line:
                match = re.search(r"['\"](.*)['\"]", line)
                if match:
                    class_name = match.group(1)
                    category_index[class_id] = class_name
    return category_index

# ラベルマップをロード
label_map = load_label_map('/back/submodule/objectDetection/label_map.pbtxt')

# saved_modelのロード
model = tf.saved_model.load("/Users/toshizumi/CLionProjects/kousen_basic_research/back/submodule/objectDetection/mobilenet/saved_model")

# 推論関数を取得
infer = model.signatures['serving_default']

# カメラ映像を取得
cap = cv2.VideoCapture(0)  # 0はデフォルトのカメラ

# FPS計測用変数
fps_list = []

while True:
    start_time = time.time()  # フレーム処理の開始時間を取得

    ret, frame = cap.read()
    if not ret:
        break

    # フレームをリサイズしてモデル入力に合わせる
    input_tensor = cv2.resize(frame, (320, 320))  # モデルに合わせたサイズに変更
    input_tensor = np.expand_dims(input_tensor, axis=0)  # バッチ次元を追加
    input_tensor = tf.convert_to_tensor(input_tensor, dtype=tf.uint8)  # テンソルに変換

    # 推論を実行
    detections = infer(input_tensor)

    # 検出結果を取得
    boxes = detections['detection_boxes'].numpy()[0]  # 検出ボックス
    scores = detections['detection_scores'].numpy()[0]  # 信頼度スコア
    classes = detections['detection_classes'].numpy()[0].astype(int)  # クラスID（整数型に変換）

    # 検出結果の描画
    h, w, _ = frame.shape
    for i in range(len(scores)):
        if scores[i] > 0.5:  # 信頼度が50%以上の場合のみ描画
            ymin, xmin, ymax, xmax = boxes[i]
            xmin, xmax, ymin, ymax = int(xmin * w), int(xmax * w), int(ymin * h), int(ymax * h)
            
            # バウンディングボックスを描画
            cv2.rectangle(frame, (xmin, ymin), (xmax, ymax), (0, 255, 0), 2)
            
            # クラス名と信頼度スコアを描画
            class_id = classes[i]
            class_name = label_map.get(class_id, 'Unknown')  # クラス名を取得
            label = f'{class_name}: {scores[i]:.2f}'
            cv2.putText(frame, label, (xmin, ymin - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)

    # FPS計算
    end_time = time.time()
    elapsed_time = end_time - start_time  # 処理にかかった時間
    fps = 1 / elapsed_time  # FPSを計算
    fps_list.append(fps)  # FPSリストに追加

    # フレームにFPSを描画
    cv2.putText(frame, f'FPS: {fps:.2f}', (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 0), 2)

    # フレームを表示
    cv2.imshow('Object Detection', frame)

    # 'q'キーで終了
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# リソースの解放
cap.release()
cv2.destroyAllWindows()

# 平均FPSを表示
average_fps = sum(fps_list) / len(fps_list)
print(f'Average FPS: {average_fps:.2f}')
