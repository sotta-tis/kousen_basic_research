import {
  Box,
  Button,
  FormControl,
  FormLabel,
  HStack,
  Input,
  NumberDecrementStepper,
  NumberIncrementStepper,
  NumberInput,
  NumberInputField,
  NumberInputStepper,
} from "@chakra-ui/react";
import { useEffect, useState } from "react";

type ConnectionFormProps = {
  ipAddress: string;
  port: string;
  standby: string;
  open: string;
  close: string;
};

const GlipperSetting = ({
  ipAddress,
  port,
  standby,
  open,
  close,
}: ConnectionFormProps) => {
  // ステートを使って入力値を管理
  const [currentIpAddress, setCurrentIpAddress] = useState(ipAddress);
  const [currentPort, setCurrentPort] = useState(port);
  const [initialStandby, setInitialStandby] = useState(standby);
  const [initialOpen, setInitialOpen] = useState(open);
  const [initialClose, setInitialClose] = useState(close);
  const [jsonResponse, setJsonResponse] = useState(null);
  const [error, setError] = useState(null);

  useEffect(() => {
    setCurrentIpAddress(ipAddress);
    setCurrentPort(port);
    setInitialStandby(standby);
    setInitialClose(close);
    setInitialOpen(open);
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
      const urlToFetch = `${process.env.NEXT_PUBLIC_BACKEND_API_ENDPOINT}/admin/glipper/set?${queryParams}`;
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

  const handleInitalSubmit = async (event: React.FormEvent) => {
    event.preventDefault();

    const params = new URLSearchParams();
    // Stateから直接URLSearchParamsに追加
    params.append("standby", initialStandby);
    params.append("open", initialOpen);
    params.append("close", initialClose);

    const queryParams = params.toString();
    try {
      // クエリパラメータでリクエストしたいURLを指定
      const urlToFetch = `${process.env.NEXT_PUBLIC_BACKEND_API_ENDPOINT}/admin/glipper/initial?${queryParams}`;
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

      <Box as="form" id="initialPosForm" onSubmit={handleInitalSubmit}>
        <HStack spacing={3}>
          <FormControl>
            <FormLabel htmlFor="standby">standby</FormLabel>
            <NumberInput
              id="standby"
              value={initialStandby}
              onChange={(valueString) => setInitialStandby(valueString)}
              min={1}
              max={65535}
            >
              <NumberInputField placeholder="Enter X" />
              <NumberInputStepper>
                <NumberIncrementStepper />
                <NumberDecrementStepper />
              </NumberInputStepper>
            </NumberInput>
          </FormControl>

          <FormControl>
            <FormLabel htmlFor="close">close</FormLabel>
            <NumberInput
              id="close"
              value={initialClose}
              onChange={(valueString) => setInitialClose(valueString)}
              min={1}
              max={65535}
            >
              <NumberInputField placeholder="Enter Z" />
              <NumberInputStepper>
                <NumberIncrementStepper />
                <NumberDecrementStepper />
              </NumberInputStepper>
            </NumberInput>
          </FormControl>

          <FormControl>
            <FormLabel htmlFor="open">open</FormLabel>
            <NumberInput
              id="open"
              value={initialOpen}
              onChange={(valueString) => setInitialOpen(valueString)}
              min={1}
              max={65535}
            >
              <NumberInputField placeholder="Enter Y" />
              <NumberInputStepper>
                <NumberIncrementStepper />
                <NumberDecrementStepper />
              </NumberInputStepper>
            </NumberInput>
          </FormControl>
        </HStack>

        <Button type="submit" colorScheme="teal" mt={4}>
          Submit
        </Button>
      </Box>
    </>
  );
};

export default GlipperSetting;
