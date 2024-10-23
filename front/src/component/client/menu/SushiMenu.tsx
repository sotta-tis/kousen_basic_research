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
  NumberInput,
  NumberInputField,
  NumberInputStepper,
  NumberIncrementStepper,
  NumberDecrementStepper,
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
  const [quantity, setQuantity] = useState(1);

  const handleQuantityChange = (value: string) => setQuantity(Number(value));

  return (
    <>
      {stock ? (
        <>
          <Box
            onClick={onOpen}
            cursor="pointer"
            textAlign="center"
            w="200px"
            justifyItems={"center"}
            borderWidth="1px"
            borderRadius="md"
            borderColor="gray.300"
            _hover={{ borderColor: "blue.400", boxShadow: "md" }}
          >
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
                  spacing={3}
                  alignItems="center"
                  justifyContent="flex-start"
                  w="full"
                >
                  <Text>枚数:</Text>
                  <NumberInput
                    aria-label="ds"
                    min={1}
                    max={stock}
                    value={quantity}
                    onChange={handleQuantityChange}
                  >
                    <NumberInputField />
                    <NumberInputStepper>
                      <NumberIncrementStepper />
                      <NumberDecrementStepper />
                    </NumberInputStepper>
                  </NumberInput>
                  <Button colorScheme="blue" onClick={onClose}>
                    注文確定
                  </Button>
                </HStack>
              </ModalFooter>
            </ModalContent>
          </Modal>
        </>
      ) : (
        <></>
      )}
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
