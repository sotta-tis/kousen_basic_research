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
  xMin: number;
  yMin: number;
  xMax: number;
  yMax: number;
  zoneR: number;
  zoneZ: number;

  rlsX: number;
  rlsY: number;
  rlsZ: number;
  rlsR: number;

  dishX: number;
  dishY: number;
  dishZ: number;
  dishR: number;

  dishRlsX: number;
  dishRlsY: number;
  dishRlsZ: number;
  dishRlsR: number;
};

const DobotSetting = ({
  x,
  y,
  z,
  r,
  xMin,
  yMin,
  xMax,
  yMax,
  zoneR,
  zoneZ,
  rlsR,
  rlsX,
  rlsY,
  rlsZ,
  dishX,
  dishY,
  dishZ,
  dishR,

  dishRlsX,
  dishRlsY,
  dishRlsZ,
  dishRlsR,
}: DobotControllerProps) => {
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
      setCoordinateZ((prevZ) => Math.max(0, Math.min(100, prevZ + increment)));
    }
  };

  // 各フォームの送信ハンドラー
  const handleSubmitLeftTop = async (e: React.FormEvent) => {
    const query = `?x=${coordinateX}&y=${coordinateY}`;
    try {
      const urlToFetch = `${process.env.NEXT_PUBLIC_BACKEND_API_ENDPOINT}/admin/zone/min${query}`;
      await fetch(`/api/proxy?url=${encodeURIComponent(urlToFetch)}`);
    } catch (error) {
      console.error("Failed to send data", error);
    }
  };

  const handleSubmitRightBottom = async (e: React.FormEvent) => {
    const query = `?x=${coordinateX}&y=${coordinateY}`;
    try {
      const urlToFetch = `${process.env.NEXT_PUBLIC_BACKEND_API_ENDPOINT}/admin/zone/max${query}`;
      await fetch(`/api/proxy?url=${encodeURIComponent(urlToFetch)}`);
    } catch (error) {
      console.error("Failed to send data", error);
    }
  };

  const handleSubmitHeightAndAngle = async (e: React.FormEvent) => {
    const query = `?r=${coordinateR}&z=${coordinateZ}`;
    try {
      const urlToFetch = `${process.env.NEXT_PUBLIC_BACKEND_API_ENDPOINT}/admin/zone/height${query}`;
      await fetch(`/api/proxy?url=${encodeURIComponent(urlToFetch)}`);
    } catch (error) {
      console.error("Failed to send data", error);
    }
  };

  const handleSubmitReturnPlace = async (e: React.FormEvent) => {
    const query = `?x=${coordinateX}&y=${coordinateY}&z=${coordinateZ}&r=${coordinateR}`;
    try {
      const urlToFetch = `${process.env.NEXT_PUBLIC_BACKEND_API_ENDPOINT}/admin/release${query}`;
      await fetch(`/api/proxy?url=${encodeURIComponent(urlToFetch)}`);
    } catch (error) {
      console.error("Failed to send data", error);
    }
  };

  const handleSubmitDishPlace = async (e: React.FormEvent) => {
    const query = `?x=${coordinateX}&y=${coordinateY}&z=${coordinateZ}&r=${coordinateR}`;
    try {
      const urlToFetch = `${process.env.NEXT_PUBLIC_BACKEND_API_ENDPOINT}/admin/zone/dish${query}`;
      await fetch(`/api/proxy?url=${encodeURIComponent(urlToFetch)}`);
    } catch (error) {
      console.error("Failed to send data", error);
    }
  };

  const handleSubmitReturnDishPlace = async (e: React.FormEvent) => {
    const query = `?x=${coordinateX}&y=${coordinateY}&z=${coordinateZ}&r=${coordinateR}`;
    try {
      const urlToFetch = `${process.env.NEXT_PUBLIC_BACKEND_API_ENDPOINT}/admin/dish/release${query}`;
      await fetch(`/api/proxy?url=${encodeURIComponent(urlToFetch)}`);
    } catch (error) {
      console.error("Failed to send data", error);
    }
  };

  return (
    <Container>
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
                  <NumberInput
                    value={coordinateX}
                    onChange={setCoordinateX}
                    min={0}
                    max={300}
                  >
                    <NumberInputField />
                    <NumberInputStepper>
                      <NumberIncrementStepper />
                      <NumberDecrementStepper />
                    </NumberInputStepper>
                  </NumberInput>
                </FormControl>

                <FormControl>
                  <FormLabel>Y</FormLabel>
                  <NumberInput
                    value={coordinateY}
                    onChange={setCoordinateY}
                    min={0}
                    max={300}
                  >
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
              <NumberInput
                value={coordinateZ}
                onChange={setCoordinateZ}
                min={0}
                max={100}
              >
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
        <VStack>
          <Box as="form" onSubmit={handleSubmitLeftTop} mb={4}>
            <HStack spacing={4} justifyContent={"start"} wrap={"nowrap"}>
              <Button
                type="submit"
                colorScheme="teal"
                size={"auto"}
                padding={2}
              >
                左上
              </Button>
              <FormControl>
                <FormLabel whiteSpace="nowrap">X : {xMin}</FormLabel>
              </FormControl>
              <FormControl>
                <FormLabel whiteSpace="nowrap">Y : {yMin}</FormLabel>
              </FormControl>
            </HStack>
          </Box>
          <Box as="form" onSubmit={handleSubmitRightBottom} mb={4}>
            <HStack spacing={4} alignContent="end">
              <Button
                type="submit"
                colorScheme="teal"
                size={"auto"}
                padding={2}
              >
                右下
              </Button>
              <FormControl>
                <FormLabel whiteSpace="nowrap">X : {xMax}</FormLabel>
              </FormControl>
              <FormControl>
                <FormLabel whiteSpace="nowrap">Y : {yMax}</FormLabel>
              </FormControl>
            </HStack>
          </Box>
          <Box as="form" onSubmit={handleSubmitHeightAndAngle} mb={4}>
            <HStack spacing={4} alignContent="end">
              <Button
                type="submit"
                colorScheme="teal"
                size={"auto"}
                padding={2}
              >
                高さと角度
              </Button>
              <FormControl>
                <FormLabel whiteSpace="nowrap">R : {zoneR}</FormLabel>
              </FormControl>
              <FormControl>
                <FormLabel whiteSpace="nowrap">Z : {zoneZ}</FormLabel>
              </FormControl>
            </HStack>
          </Box>
          <Box as="form" onSubmit={handleSubmitReturnPlace} mb={4}>
            <HStack spacing={4} alignContent="end">
              <Button
                type="submit"
                colorScheme="teal"
                size={"auto"}
                padding={2}
              >
                寿司移動場所
              </Button>
              <FormControl>
                <FormLabel whiteSpace="nowrap">X : {rlsX}</FormLabel>
              </FormControl>
              <FormControl>
                <FormLabel whiteSpace="nowrap">Y : {rlsY}</FormLabel>
              </FormControl>
              <FormControl>
                <FormLabel whiteSpace="nowrap">Z : {rlsZ}</FormLabel>
              </FormControl>
              <FormControl>
                <FormLabel whiteSpace="nowrap">R : {rlsR}</FormLabel>
              </FormControl>
            </HStack>
          </Box>
          <Box as="form" onSubmit={handleSubmitDishPlace} mb={4}>
            <HStack spacing={4} alignContent="end">
              <Button
                type="submit"
                colorScheme="teal"
                size={"auto"}
                padding={2}
              >
                皿掴む
              </Button>
              <FormControl>
                <FormLabel whiteSpace="nowrap">X : {dishX}</FormLabel>
              </FormControl>
              <FormControl>
                <FormLabel whiteSpace="nowrap">Y : {dishY}</FormLabel>
              </FormControl>
              <FormControl>
                <FormLabel whiteSpace="nowrap">Z : {dishZ}</FormLabel>
              </FormControl>
              <FormControl>
                <FormLabel whiteSpace="nowrap">R : {dishR}</FormLabel>
              </FormControl>
            </HStack>
          </Box>
          <Box as="form" onSubmit={handleSubmitReturnDishPlace} mb={4}>
            <HStack spacing={4} alignContent="end">
              <Button
                type="submit"
                colorScheme="teal"
                size={"auto"}
                padding={2}
              >
                皿リリース
              </Button>
              <FormControl>
                <FormLabel whiteSpace="nowrap">X : {dishRlsX}</FormLabel>
              </FormControl>
              <FormControl>
                <FormLabel whiteSpace="nowrap">Y : {dishRlsY}</FormLabel>
              </FormControl>
              <FormControl>
                <FormLabel whiteSpace="nowrap">Z : {dishRlsZ}</FormLabel>
              </FormControl>
              <FormControl>
                <FormLabel whiteSpace="nowrap">R : {dishRlsR}</FormLabel>
              </FormControl>
            </HStack>
          </Box>
        </VStack>
      </Box>
    </Container>
  );
};

export default DobotSetting;
