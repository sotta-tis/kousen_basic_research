import { useEffect, useState } from "react";

export const customFetch = (url: string) => {
  const [jsonResponse, setJsonResponse] = useState(null);
  const [error, setError] = useState(null);

  useEffect(() => {
    const fetchData = async () => {
      try {
        // クエリパラメータでリクエストしたいURLを指定
        const urlToFetch = process.env.NEXT_PUBLIC_BACKEND_API_ENDPOINT + url;
        const response = await fetch(
          `/api/proxy?url=${encodeURIComponent(urlToFetch)}`
        );

        if (!response.ok) {
          throw new Error(`Failed to fetch data: ${response.statusText}`);
        }

        const data = await response.json();
        setJsonResponse(data);
      } catch (error) {
        setError(error.message);
      }
    };

    fetchData();
  }, []);

  const res = jsonResponse;

  return {
    res,
    error,
  };
};
