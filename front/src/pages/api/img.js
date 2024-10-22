// pages/api/getImage.js
import axios from "axios";

export default async function handler(req, res) {
  const { scale } = req.query;

  try {
    // Drogonバックエンドのエンドポイントにリクエストを送信
    const response = await axios.get("http://127.0.0.1:8055/admin/image", {
      params: { scale }, // クエリパラメータを追加
      responseType: "arraybuffer",
    });

    // 画像データをBase64エンコードして返す
    const imageBuffer = Buffer.from(response.data, "binary").toString("base64");
    res.setHeader("Content-Type", "image/jpeg");
    res.setHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    res.setHeader("Pragma", "no-cache");
    res.setHeader("Expires", "0");
    res.status(200).send(imageBuffer);
  } catch (error) {
    console.error("Error fetching image:", error);
    res.status(500).json({ message: "Failed to fetch image" });
  }
}
