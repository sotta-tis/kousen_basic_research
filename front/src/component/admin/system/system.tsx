import React from "react";
import { Box, Text, Button, Stack, VStack, HStack } from "@chakra-ui/react";
import { useRouter } from "next/router";

interface Props {
  dobot: {
    host: string;
    port: number;
    r: number;
    x: number;
    y: number;
    z: number;
  };
  glip: {
    close: number;
    host: string;
    open: number;
    port: number;
    standby: number;
  };
}

const SystemPanel: React.FC<Props> = ({ dobot, glip }) => {
  const router = useRouter();

  // 設定画面に遷移する関数
  const navigateToSettings = (path: string) => {
    router.push(path); // pathに応じた設定画面に遷移
  };

  return (
    <Box p={4} borderWidth={1} borderRadius="lg" shadow="md">
      <VStack spacing={6} align="stretch">
        {/* Dobotの情報 */}
        <Box>
          <Text fontSize="xl" fontWeight="bold">
            Dobot Settings
          </Text>
          <Stack spacing={2}>
            <Text>Host: {dobot.host}</Text>
            <Text>Port: {dobot.port}</Text>
            <Text>R: {dobot.r}</Text>
            <Text>X: {dobot.x}</Text>
            <Text>Y: {dobot.y}</Text>
            <Text>Z: {dobot.z}</Text>
          </Stack>
          <Button
            colorScheme="teal"
            mt={2}
            onClick={() => navigateToSettings("/admin/dobot/setting")}
          >
            Dobot 設定を変更
          </Button>
        </Box>

        {/* Glipの情報 */}
        <Box>
          <Text fontSize="xl" fontWeight="bold">
            Glip Settings
          </Text>
          <Stack spacing={2}>
            <Text>Host: {glip.host}</Text>
            <Text>Port: {glip.port}</Text>
            <Text>Close: {glip.close}</Text>
            <Text>Open: {glip.open}</Text>
            <Text>Standby: {glip.standby}</Text>
          </Stack>
          <Button
            colorScheme="teal"
            mt={2}
            onClick={() => navigateToSettings("/admin/glipper/setting")}
          >
            Glip 設定を変更
          </Button>
        </Box>
      </VStack>
    </Box>
  );
};

export default SystemPanel;
