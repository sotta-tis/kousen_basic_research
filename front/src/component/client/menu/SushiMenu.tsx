import React, { useState } from "react";
import {
  Box,
  Image,
  Text,
  HStack,
  useDisclosure,
  Modal,
  ModalOverlay,
  ModalContent,
  ModalHeader,
  ModalFooter,
  ModalBody,
  ModalCloseButton,
  Button,
  useToast,
} from "@chakra-ui/react";

interface ItemProps {
  imageSrc: string;
  price: string;
  name: string;
  stock: number;
}

interface MenuProps {
  count: number;
  menu: { [key: string]: number };
}

const SushiItem: React.FC<ItemProps> = ({ imageSrc, price, name, stock }) => {
  const { isOpen, onOpen, onClose } = useDisclosure();
  const [loading, setLoading] = useState(false);
  const toast = useToast();

  const order = async () => {
    setLoading(true);
    const urlToFetch = `${process.env.NEXT_PUBLIC_BACKEND_API_ENDPOINT}/order`;
    try {
      const res = await fetch(
        `/api/proxy?url=${encodeURIComponent(urlToFetch)}`
      );
      if (res.ok) {
        toast({
          title: "ご注文ありがとうございます",
          status: "success",
          duration: 3000,
          isClosable: true,
        });
      } else {
        throw new Error("Failed Ordering");
        onClose;
      }
    } catch (error) {
      console.error("Failed to load image:", error);
      toast({
        title: "Error Ordering",
        status: "error",
        duration: 3000,
        isClosable: true,
      });
      onClose;
    } finally {
      setLoading(false);
      onClose;
    }
  };

  return (
    <>
      <Box
        onClick={stock ? onOpen : undefined}
        cursor={stock ? "pointer" : "not-allowed"}
        textAlign="center"
        w="200px"
        justifyItems={"center"}
        borderWidth="1px"
        borderRadius="md"
        borderColor="gray.300"
        position="relative"
        _hover={stock ? { borderColor: "blue.400", boxShadow: "md" } : {}}
        opacity={stock ? 1 : 0.5}
      >
        {/* 在庫がない場合に「在庫なし」のテキストを被せる */}
        {!stock && (
          <Box
            position="absolute"
            top="0"
            left="0"
            w="full"
            h="full"
            bg="rgba(0, 0, 0, 0.6)"
            display="flex"
            alignItems="center"
            justifyContent="center"
            color="white"
            fontSize="lg"
            fontWeight="bold"
            borderRadius="md"
          >
            在庫なし
          </Box>
        )}
        <Image
          src={imageSrc}
          alt={name}
          boxSize="150px"
          objectFit="scale-down"
        />
        <Text mt={2}>{name}</Text>
        <Text fontWeight="bold">{price}円</Text>
      </Box>

      <Modal isOpen={isOpen} onClose={onClose}>
        <ModalOverlay />
        <ModalContent>
          <ModalHeader>{name}</ModalHeader>
          <ModalCloseButton />
          <ModalBody>
            <HStack justifyContent={"center"}>
              <Image
                src={imageSrc}
                alt={name}
                boxSize="150px"
                objectFit="scale-down"
              />
            </HStack>
          </ModalBody>

          <ModalFooter>
            <HStack
              spacing={5}
              alignItems="center"
              w="full"
              justifyContent="center"
            >
              <Text>残り：{stock} 枚</Text>
              <Button colorScheme="blue" onClick={order} disabled={!stock}>
                注文確定
              </Button>
            </HStack>
          </ModalFooter>
        </ModalContent>
      </Modal>
    </>
  );
};

const SushiMenu: React.FC<MenuProps> = ({ count, menu }) => {
  const items = {
    6: {
      imageSrc: "/salmon.png",
      price: "100",
      name: "えび",
      stock: menu["6"],
    },
    1: {
      imageSrc: "/ikura.png",
      price: "100",
      name: "いくら",
      stock: menu["1"],
    },
    2: {
      imageSrc: "/maguro.png",
      price: "100",
      name: "マグロ",
      stock: menu["2"],
    },
    3: { imageSrc: "/ika.png", price: "100", name: "いか", stock: menu["3"] },
    4: { imageSrc: "/ika.png", price: "100", name: "うに", stock: menu["4"] },
    5: { imageSrc: "/ika.png", price: "100", name: "たまご", stock: menu["5"] },
  };

  return (
    <HStack spacing={8} align="flex-start" wrap={"wrap"} justify={"center"}>
      {Object.values(items).map((item, index) => (
        <SushiItem
          key={index}
          imageSrc={item.imageSrc}
          price={item.price}
          name={item.name}
          stock={item.stock}
        />
      ))}
    </HStack>
  );
};

export default SushiMenu;
