import React, { useState, useEffect } from "react";
import {
  Box,
  Button,
  Container,
  FormControl,
  FormLabel,
  NumberInput,
  NumberInputField,
  NumberInputStepper,
  NumberIncrementStepper,
  NumberDecrementStepper,
  HStack,
  VStack,
  Center,
} from "@chakra-ui/react";

type DobotControllerProps = {
  x: number;
  y: number;
  z: number;
  r: number;
};

const DobotController = ({ x, y, z, r }: DobotControllerProps) => {
  const [coordinateX, setCoordinateX] = useState(x);
  const [coordinateY, setCoordinateY] = useState(y);
  const [coordinateZ, setCoordinateZ] = useState(z);
  const [coordinateR, setCoordinateR] = useState(r);
  const [intervalId, setIntervalId] = useState<NodeJS.Timeout | null>(null);

  // 値を変更すると自動的にAPIに送信
  const sendCoordinatesToAPI = async (
    x: number,
    y: number,
    z: number,
    r: number
  ) => {
    const query = `?x=${x}&y=${y}&z=${z}&r=${r}`;
    try {
      const urlToFetch = `${process.env.NEXT_PUBLIC_BACKEND_API_ENDPOINT}/admin/dobot/go${query}`;
      await fetch(`/api/proxy?url=${encodeURIComponent(urlToFetch)}`);
    } catch (error) {
      console.error("Failed to send data", error);
    }
  };

  // 値が変更されるたびにAPIへ送信
  useEffect(() => {
    sendCoordinatesToAPI(coordinateX, coordinateY, coordinateZ, coordinateR);
  }, [coordinateX, coordinateY, coordinateZ, coordinateR]);

  // 長押し用の関数
  const handleLongPressStart = (axis: string, increment: number) => {
    if (!intervalId) {
      const newIntervalId = setInterval(() => {
        handleCoordinateChange(axis, increment);
      }, 100); // 100ミリ秒ごとに値を更新
      setIntervalId(newIntervalId);
    }
  };

  // 長押し解除時の関数
  const handleLongPressEnd = () => {
    if (intervalId) {
      clearInterval(intervalId);
      setIntervalId(null);
    }
  };

  // 値を変更する共通関数（XとYは5ずつ、Zは1ずつ変更）
  const handleCoordinateChange = (axis: string, increment: number) => {
    if (axis === "x") {
      setCoordinateX((prevX) =>
        Math.max(0, Math.min(300, prevX + increment * 5))
      );
    } else if (axis === "y") {
      setCoordinateY((prevY) =>
        Math.max(0, Math.min(300, prevY + increment * 5))
      );
    } else if (axis === "z") {
      setCoordinateZ((prevZ) => Math.max(0, Math.min(80, prevZ + increment)));
    }
  };

  return (
    <Container>
      {/* 全体を囲む枠 */}
      <Box
        p={6}
        border="2px solid"
        borderColor="gray.300"
        borderRadius="md"
        bg="gray.50"
      >
        <HStack spacing={12} alignItems="flex-start">
          {/* XYのフォームとコントローラー */}
          <VStack spacing={6}>
            {/* XY軸フォーム */}
            <VStack spacing={4}>
              <HStack spacing={4}>
                <FormControl>
                  <FormLabel>X</FormLabel>
                  <NumberInput value={coordinateX} min={-100} max={300}>
                    <NumberInputField />
                    <NumberInputStepper>
                      <NumberIncrementStepper />
                      <NumberDecrementStepper />
                    </NumberInputStepper>
                  </NumberInput>
                </FormControl>

                <FormControl>
                  <FormLabel>Y</FormLabel>
                  <NumberInput value={coordinateY} min={-100} max={300}>
                    <NumberInputField />
                    <NumberInputStepper>
                      <NumberIncrementStepper />
                      <NumberDecrementStepper />
                    </NumberInputStepper>
                  </NumberInput>
                </FormControl>
              </HStack>
            </VStack>

            {/* XYの矢印型コントローラー */}
            <Box position="relative" w="200px" h="200px">
              <Center>
                <VStack>
                  {/* 上方向 (Y軸) */}
                  <Button
                    onMouseDown={() => handleLongPressStart("y", 1)}
                    onMouseUp={handleLongPressEnd}
                    onMouseLeave={handleLongPressEnd}
                    aria-label="up"
                    colorScheme="blue" // Y軸を青色に
                  >
                    ↑
                  </Button>

                  {/* 左右 (X軸) */}
                  <HStack>
                    <Button
                      onMouseDown={() => handleLongPressStart("x", -1)}
                      onMouseUp={handleLongPressEnd}
                      onMouseLeave={handleLongPressEnd}
                      aria-label="left"
                      colorScheme="green" // X軸を緑色に
                    >
                      ←
                    </Button>
                    <Box width="50px" />
                    <Button
                      onMouseDown={() => handleLongPressStart("x", 1)}
                      onMouseUp={handleLongPressEnd}
                      onMouseLeave={handleLongPressEnd}
                      aria-label="right"
                      colorScheme="green"
                    >
                      →
                    </Button>
                  </HStack>

                  {/* 下方向 (Y軸) */}
                  <Button
                    onMouseDown={() => handleLongPressStart("y", -1)}
                    onMouseUp={handleLongPressEnd}
                    onMouseLeave={handleLongPressEnd}
                    aria-label="down"
                    colorScheme="blue"
                  >
                    ↓
                  </Button>
                </VStack>
              </Center>
            </Box>
          </VStack>

          {/* Zのフォームとコントローラー */}
          <VStack spacing={6}>
            {/* Z軸フォーム */}
            <FormControl>
              <FormLabel>Z</FormLabel>
              <NumberInput value={coordinateZ} min={0} max={100}>
                <NumberInputField />
                <NumberInputStepper>
                  <NumberIncrementStepper />
                  <NumberDecrementStepper />
                </NumberInputStepper>
              </NumberInput>
            </FormControl>

            {/* Z軸上下コントローラー */}
            <Box position="relative" w="200px" h="100px">
              <Center>
                <VStack>
                  {/* Z軸 上 */}
                  <Button
                    onMouseDown={() => handleLongPressStart("z", 1)}
                    onMouseUp={handleLongPressEnd}
                    onMouseLeave={handleLongPressEnd}
                    aria-label="Z-up"
                    colorScheme="red" // Z軸を赤色に
                  >
                    Z ↑
                  </Button>

                  {/* Z軸 下 */}
                  <Button
                    onMouseDown={() => handleLongPressStart("z", -1)}
                    onMouseUp={handleLongPressEnd}
                    onMouseLeave={handleLongPressEnd}
                    aria-label="Z-down"
                    colorScheme="red"
                  >
                    Z ↓
                  </Button>
                </VStack>
              </Center>
            </Box>
          </VStack>
        </HStack>
      </Box>
    </Container>
  );
};

export default DobotController;
