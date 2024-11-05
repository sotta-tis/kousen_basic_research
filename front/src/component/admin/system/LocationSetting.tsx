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
import DobotSetting from "./DobotSetting";

type ConnectionFormProps = {
  x: string;
  y: string;
  z: string;
  r: string;

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

const ImageSetting = ({
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
}: ConnectionFormProps) => {
  const [jsonResponse, setJsonResponse] = useState(null);
  const [error, setError] = useState(null);
  const [imageSrc, setImageSrc] = useState(null);
  const [loading, setLoading] = useState(false);
  const [boundingBox, setBoundingBox] = useState(null);
  const canvasRef = useRef(null);
  const startPos = useRef({ x: 0, y: 0 });
  const toast = useToast();

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
    <>
      <Box>
        <DobotSetting
          x={parseInt(x)}
          y={parseInt(y)}
          z={parseInt(z)}
          r={parseInt(r)}
          xMin={xMin}
          xMax={xMax}
          yMax={yMax}
          yMin={yMin}
          zoneR={zoneR}
          zoneZ={zoneZ}
          rlsR={rlsR}
          rlsX={rlsX}
          rlsY={rlsY}
          rlsZ={rlsZ}
          dishX={dishX}
          dishY={dishY}
          dishR={dishR}
          dishZ={dishZ}
          dishRlsX={dishRlsX}
          dishRlsY={dishRlsY}
          dishRlsR={dishRlsR}
          dishRlsZ={dishRlsZ}
        />
      </Box>
    </>
  );
};

export default ImageSetting;
