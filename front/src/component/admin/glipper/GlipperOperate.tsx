import { Box, Button, HStack } from "@chakra-ui/react";
import { useEffect, useState } from "react";

const GlipperSetting = () => {
  // ステートを使って入力値を管理
  const [jsonResponse, setJsonResponse] = useState(null);
  const [error, setError] = useState(null);

  const handleStandby = async (event: React.FormEvent) => {
    event.preventDefault();

    try {
      // クエリパラメータでリクエストしたいURLを指定
      const urlToFetch = `${process.env.NEXT_PUBLIC_BACKEND_API_ENDPOINT}/admin/glipper/do/standby`;
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

  const handleClose = async (event: React.FormEvent) => {
    event.preventDefault();
    try {
      // クエリパラメータでリクエストしたいURLを指定
      const urlToFetch = `${process.env.NEXT_PUBLIC_BACKEND_API_ENDPOINT}/admin/glipper/do/close`;
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
  const handleOpen = async (event: React.FormEvent) => {
    event.preventDefault();
    try {
      // クエリパラメータでリクエストしたいURLを指定
      const urlToFetch = `${process.env.NEXT_PUBLIC_BACKEND_API_ENDPOINT}/admin/glipper/do/open`;
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

  const handleDishStandby = async (event: React.FormEvent) => {
    event.preventDefault();

    try {
      // クエリパラメータでリクエストしたいURLを指定
      const urlToFetch = `${process.env.NEXT_PUBLIC_BACKEND_API_ENDPOINT}/admin/glipper/dish/standby`;
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

  const handleDishClose = async (event: React.FormEvent) => {
    event.preventDefault();
    try {
      // クエリパラメータでリクエストしたいURLを指定
      const urlToFetch = `${process.env.NEXT_PUBLIC_BACKEND_API_ENDPOINT}/admin/glipper/dish/close`;
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
  const handleDishOpen = async (event: React.FormEvent) => {
    event.preventDefault();
    try {
      // クエリパラメータでリクエストしたいURLを指定
      const urlToFetch = `${process.env.NEXT_PUBLIC_BACKEND_API_ENDPOINT}/admin/glipper/dish/open`;
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

  return (
    <>
      <HStack spacing={3}>
        <Box as="form" id="connectionForm" onSubmit={handleStandby}>
          {/* Submitボタン */}
          <Button type="submit" colorScheme="teal" mt={4}>
            待機
          </Button>
        </Box>

        <Box as="form" id="initialPosForm" onSubmit={handleClose}>
          <Button type="submit" colorScheme="teal" mt={4}>
            掴む
          </Button>
        </Box>
        <Box as="form" id="initialPosForm" onSubmit={handleOpen}>
          <Button type="submit" colorScheme="teal" mt={4}>
            はなす
          </Button>
        </Box>
      </HStack>
      <HStack>
        <Box as="form" id="connectionForm" onSubmit={handleDishStandby}>
          {/* Submitボタン */}
          <Button type="submit" colorScheme="teal" mt={4}>
            待機
          </Button>
        </Box>

        <Box as="form" id="initialPosForm" onSubmit={handleDishClose}>
          <Button type="submit" colorScheme="teal" mt={4}>
            掴む
          </Button>
        </Box>

        <Box as="form" id="initialPosForm" onSubmit={handleDishOpen}>
          <Button type="submit" colorScheme="teal" mt={4}>
            はなす
          </Button>
        </Box>
      </HStack>
    </>
  );
};

export default GlipperSetting;
