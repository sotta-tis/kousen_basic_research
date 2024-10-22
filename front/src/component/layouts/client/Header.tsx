import { Flex, Box, Button } from "@chakra-ui/react";
import { useRouter } from "next/router";

const Header = () => {
  const router = useRouter();

  const handleNavigation = (path: string) => {
    router.push(path);
  };

  return (
    <Flex
      as="nav"
      align="center"
      justify="center" // ボタンを中央に揃える
      height="60px" // ヘッダーの高さを指定
      backgroundColor="#001f3f" // 紺色に設定
      color="white"
      boxShadow="md"
    >
      {/* ナビゲーションボタン */}
      <Flex gap="4" align="center" height="100%">
        {" "}
        <Button
          variant="ghost"
          colorScheme="whiteAlpha"
          height="100%"
          onClick={() => handleNavigation("/")}
        >
          メニュー
        </Button>
        <Button
          variant="ghost"
          colorScheme="whiteAlpha"
          height="100%"
          onClick={() => handleNavigation("/")}
        >
          注文履歴
        </Button>
      </Flex>
    </Flex>
  );
};

export default Header;
