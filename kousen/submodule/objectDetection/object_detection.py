import tensorflow as tf
import numpy as np
import cv2
from PIL import Image
import matplotlib.pyplot as plt
from object_detection.utils import label_map_util

# モデルの読み込み (事前に保存したモデルパス)
model_path = '/Users/toshizumi/CLionProjects/kousen_basic_research/kousen/submodule/objectDetection/mobilenet/saved_model'
model = tf.saved_model.load(model_path)

# label_map.pbtxt のパス
label_map_path = '/Users/toshizumi/CLionProjects/kousen_basic_research/kousen/submodule/objectDetection/label_map.pbtxt'

# ラベルマップの読み込み
category_index = label_map_util.create_category_index_from_labelmap(label_map_path, use_display_name=True)

# 画像の読み込みと前処理
def load_image_into_numpy_array(path):
    return np.array(Image.open(path))

image_path = '/Users/toshizumi/CLionProjects/kousen_basic_research/kousen/submodule/objectDetection/IMG_0046 (2).jpeg'
image_np = load_image_into_numpy_array(image_path)

# 画像のサイズ取得
input_tensor = tf.convert_to_tensor(image_np)
input_tensor = input_tensor[tf.newaxis, ...]  # バッチ次元を追加

# 推論を実行
model_fn = model.signatures['serving_default']
output_dict = model_fn(input_tensor)

# 推論結果の取り出し
num_detections = int(output_dict.pop('num_detections'))
detection_boxes = output_dict['detection_boxes'][0].numpy()
detection_classes = output_dict['detection_classes'][0].numpy().astype(np.int64)
detection_scores = output_dict['detection_scores'][0].numpy()

# スコアの閾値を設定 (例: 0.5)
THRESHOLD = 0.5

# バウンディングボックスを描画
def draw_bounding_boxes(image_np, boxes, classes, scores, category_index, threshold=0.5):
    for i in range(len(boxes)):
        if scores[i] > threshold:
            box = boxes[i]
            class_id = classes[i]
            if class_id in category_index:
                class_name = category_index[class_id]['name']
            else:
                class_name = 'N/A'
            ymin, xmin, ymax, xmax = box

            # 画像の座標に変換
            im_height, im_width, _ = image_np.shape
            (left, right, top, bottom) = (xmin * im_width, xmax * im_width, ymin * im_height, ymax * im_height)

            # バウンディングボックスを描画
            cv2.rectangle(image_np, (int(left), int(top)), (int(right), int(bottom)), (0, 255, 0), 2)
            # クラス名を表示
            cv2.putText(image_np, f'{class_name}: {scores[i]:.2f}', (int(left), int(top) - 10),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.9, (0, 255, 0), 2)

# バウンディングボックスを描画
draw_bounding_boxes(image_np, detection_boxes, detection_classes, detection_scores, category_index, threshold=THRESHOLD)

# 画像を表示
plt.figure(figsize=(12, 8))
plt.imshow(image_np)
plt.show()

# 保存する場合
output_image_path = 'output_image.jpg'
Image.fromarray(image_np).save(output_image_path)
