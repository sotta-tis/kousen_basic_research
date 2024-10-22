import {
  Box,
  Button,
  FormControl,
  FormLabel,
  Input,
  NumberInput,
  NumberInputField,
} from "@chakra-ui/react";
import { useEffect, useState } from "react";

type ConnectionFormProps = {
  ipAddress: string;
  port: string;
};

const DobotSetting = ({ ipAddress, port }: ConnectionFormProps) => {
  // ステートを使って入力値を管理
  const [currentIpAddress, setCurrentIpAddress] = useState(ipAddress);
  const [currentPort, setCurrentPort] = useState(port);
  const [jsonResponse, setJsonResponse] = useState(null);
  const [error, setError] = useState(null);

  useEffect(() => {
    setCurrentIpAddress(ipAddress);
    setCurrentPort(port);
  }, [ipAddress, port]);

  const handleSubmit = async (event: React.FormEvent) => {
    event.preventDefault();

    const params = new URLSearchParams();
    // Stateから直接URLSearchParamsに追加
    params.append("ipAddress", currentIpAddress);
    params.append("port", currentPort);

    const queryParams = params.toString();
    try {
      // クエリパラメータでリクエストしたいURLを指定
      const urlToFetch = `${process.env.NEXT_PUBLIC_BACKEND_API_ENDPOINT}/admin/dobot/set?${queryParams}`;
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
    <Box as="form" id="connectionForm" onSubmit={handleSubmit}>
      {/* IP Addressの入力 */}
      <FormControl isRequired>
        <FormLabel htmlFor="ipAddress">IP Address</FormLabel>
        <Input
          type="text"
          id="ipAddress"
          value={currentIpAddress}
          onChange={(e) => setCurrentIpAddress(e.target.value)}
          placeholder="Enter IP Address"
        />
      </FormControl>

      {/* Portの入力 */}
      <FormControl isRequired mt={4}>
        <FormLabel htmlFor="port">Port</FormLabel>
        <NumberInput
          id="port"
          value={currentPort}
          onChange={(valueString) => setCurrentPort(valueString)}
          min={1}
          max={65535}
        >
          <NumberInputField placeholder="Enter Port" />
        </NumberInput>
      </FormControl>

      {/* Submitボタン */}
      <Button type="submit" colorScheme="teal" mt={4}>
        Submit
      </Button>
    </Box>
  );
};

export default DobotSetting;
