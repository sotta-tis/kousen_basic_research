import { Flex, Icon, Text } from "@chakra-ui/react";
import { useRouter } from "next/router";
import NextLink from "next/link";

interface NavItemProps {
  path: string;
  title: string;
  depth?: number;
  index: number;
}

const NavItem: React.FC<NavItemProps> = ({ path, title, depth = 0, index }) => {
  const router = useRouter();

  return (
    <NextLink href={path} passHref key={index}>
      <Flex
        py={2}
        px={4 + depth * 3}
        fontSize="sm"
        color="white"
        align="center"
        cursor="pointer"
        bg={path === router.pathname ? "gray.500" : "gray.600"}
        _hover={{
          bg: "blue.400",
          color: "white",
          ".sidebar-icon": {
            color: "white",
          },
        }}
      >
        <Text>{title}</Text>
      </Flex>
    </NextLink>
  );
};

export default NavItem;
