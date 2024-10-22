// pages/api/proxy.js
export default async function handler(req, res) {
  const { url } = req.query; // クエリパラメータからURLを取得

  // URLがない場合は400エラーを返す
  if (!url) {
    return res.status(400).json({ error: "URL is required" });
  }

  try {
    // fetchを使ってサーバー側から指定されたURLにリクエストを送る
    const apiResponse = await fetch(url);

    // fetchが失敗した場合のエラーチェック
    if (!apiResponse.ok) {
      throw new Error(`Failed to fetch ${url}, status: ${apiResponse.status}`);
    }

    // レスポンスデータを取得してクライアントに返す
    const data = await apiResponse.json();
    res.status(200).json(data);
  } catch (error) {
    // エラーハンドリング
    res.status(500).json({ error: error.message });
  }
}
