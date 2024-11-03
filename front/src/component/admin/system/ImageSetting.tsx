// pages/index.js
import { useState, useRef, useEffect } from "react";
import {
  Box,
  Button,
  Container,
  Heading,
  Text,
  Center,
  Stack,
  Spinner,
  FormControl,
  FormLabel,
  HStack,
  NumberInput,
  NumberInputField,
  NumberInputStepper,
  NumberIncrementStepper,
  NumberDecrementStepper,
  useToast,
} from "@chakra-ui/react";
import DobotController from "../dobot/controller/DobotController";

type ConnectionFormProps = {
  x: string;
  y: string;
  z: string;
  r: string;
  scale: number;
};

const ImageSetting = ({ x, y, z, r, scale }: ConnectionFormProps) => {
  const [numericValue, setNumericValue] = useState(scale);
  const [jsonResponse, setJsonResponse] = useState(null);
  const [error, setError] = useState(null);
  const [imageSrc, setImageSrc] = useState(null);
  const [loading, setLoading] = useState(false);
  const [boundingBox, setBoundingBox] = useState(null);
  const canvasRef = useRef(null);
  const startPos = useRef({ x: 0, y: 0 });
  const toast = useToast();

  console.log(scale);

  const fetchImage = async () => {
    setLoading(true);
    try {
      // 数値をクエリパラメータとして送信
      const res = await fetch(`/api/img?scale=${numericValue}`);
      const base64Image = await res.text();
      setImageSrc(`data:image/jpeg;base64,${base64Image}`);
      setBoundingBox(null); // リセット
    } catch (error) {
      console.error("Failed to load image:", error);
    } finally {
      setLoading(false);
    }
  };

  const handleMouseDown = (e) => {
    const rect = canvasRef.current.getBoundingClientRect();
    startPos.current = { x: e.clientX - rect.left, y: e.clientY - rect.top };
  };

  const handleMouseUp = (e) => {
    const rect = canvasRef.current.getBoundingClientRect();
    const endPos = { x: e.clientX - rect.left, y: e.clientY - rect.top };
    setBoundingBox({
      x: Math.min(startPos.current.x, endPos.x),
      y: Math.min(startPos.current.y, endPos.y),
      width: Math.abs(endPos.x - startPos.current.x),
      height: Math.abs(endPos.y - startPos.current.y),
    });
  };

  const sendBoundingBox = async () => {
    if (!boundingBox) return;

    const { x, y, width, height } = boundingBox;
    const params = new URLSearchParams();
    params.append("x", x);
    params.append("y", y);
    params.append("width", width);
    params.append("height", height);

    const queryParams = params.toString();
    const urlToFetch = `${process.env.NEXT_PUBLIC_BACKEND_API_ENDPOINT}/admin/dobot/set/img/location?${queryParams}`;

    try {
      const res = await fetch(
        `/api/proxy?url=${encodeURIComponent(urlToFetch)}`
      );
      if (res.ok) {
        toast({
          title: "Bounding box sent successfully!",
          status: "success",
          duration: 3000,
          isClosable: true,
        });
      } else {
        throw new Error("Failed to send bounding box");
      }
    } catch (error) {
      console.error(error);
      toast({
        title: "Error sending bounding box",
        status: "error",
        duration: 3000,
        isClosable: true,
      });
    }
  };

  useEffect(() => {
    if (!imageSrc || !canvasRef.current) return;

    const canvas = canvasRef.current;
    const ctx = canvas.getContext("2d");
    const image = new Image();

    image.onload = () => {
      canvas.width = image.width;
      canvas.height = image.height;
      ctx.clearRect(0, 0, canvas.width, canvas.height);
      ctx.drawImage(image, 0, 0);

      if (boundingBox) {
        ctx.strokeStyle = "red";
        ctx.lineWidth = 2;
        ctx.strokeRect(
          boundingBox.x,
          boundingBox.y,
          boundingBox.width,
          boundingBox.height
        );
      }
    };
    image.src = imageSrc;
  }, [imageSrc, boundingBox]);

  return (
    <Container maxW="container.md" py={8}>
      <Heading as="h1" mb={6} textAlign="center">
        Dobot Operate with Image Viewer
      </Heading>

      <Box>
        <DobotController
          x={parseInt(x)}
          y={parseInt(y)}
          z={parseInt(z)}
          r={parseInt(r)}
        />
      </Box>

      <FormControl mb={4}>
        <FormLabel htmlFor="numericValue">
          Numeric Value for Image Fetch
        </FormLabel>
        <NumberInput
          value={numericValue}
          onChange={(valueString) => setNumericValue(parseFloat(valueString))}
          min={0}
          max={1}
          step={0.1}
          precision={2}
        >
          <NumberInputField />
          <NumberInputStepper>
            <NumberIncrementStepper />
            <NumberDecrementStepper />
          </NumberInputStepper>
        </NumberInput>
      </FormControl>
      <Center>
        <Button
          colorScheme="teal"
          onClick={fetchImage}
          mb={4}
          isLoading={loading}
        >
          Fetch Image
        </Button>
      </Center>
      <Box
        position="relative"
        borderWidth="1px"
        borderRadius="md"
        overflow="hidden"
      >
        <canvas
          ref={canvasRef}
          style={{
            display: imageSrc ? "block" : "none",
            maxWidth: "100%",
            borderRadius: "8px",
          }}
          onMouseDown={handleMouseDown}
          onMouseUp={handleMouseUp}
        />
        {loading && (
          <Center p={10}>
            <Spinner size="xl" />
          </Center>
        )}
      </Box>
      {boundingBox && (
        <Stack
          spacing={2}
          mt={4}
          p={4}
          borderWidth="1px"
          borderRadius="md"
          bg="gray.50"
        >
          <Text fontWeight="bold">Bounding Box Coordinates</Text>
          <Text>
            X: {boundingBox.x}, Y: {boundingBox.y}
          </Text>
          <Text>
            Width: {boundingBox.width}, Height: {boundingBox.height}
          </Text>
          <Button colorScheme="blue" onClick={sendBoundingBox} mt={2}>
            Send Bounding Box
          </Button>
        </Stack>
      )}
    </Container>
  );
};

export default ImageSetting;
