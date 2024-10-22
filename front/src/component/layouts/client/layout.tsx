import { Box, Flex } from "@chakra-ui/react";
import Header from "./Header";

const Layout = ({ children }) => {
  return (
    <Flex direction="column" minHeight="100vh">
      {/* 上部にヘッダー */}
      <Header />

      {/* メインコンテンツエリア */}
      <Box flex="1" padding="1rem" backgroundColor="gray.100">
        {children}
      </Box>

      {/* フッター（必要に応じて追加） */}
      <Box
        as="footer"
        textAlign="center"
        padding="1rem"
        backgroundColor="#001f3f"
        color="white"
      >
        @ 大阪公立大学高専　くま寿司
      </Box>
    </Flex>
  );
};

export default Layout;
