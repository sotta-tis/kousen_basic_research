import { ReactNode } from "react";
import {
  Flex,
  Box,
  Accordion,
  AccordionItem,
  AccordionButton,
  AccordionPanel,
  AccordionIcon,
} from "@chakra-ui/react";
import NavItem from "./item";

interface AdminLayoutProps {
  children: ReactNode;
  pageKey?: number;
}

const AdminLayout: React.FC<AdminLayoutProps> = ({ children, pageKey }) => {
  return (
    <Flex height="100vh">
      {/* 左側のメニューバー */}
      <Box w="250px" bg="gray.800" color="white" p={5}>
        <Accordion defaultIndex={[pageKey]} allowMultiple>
          {/* システム設定 */}
          <AccordionItem border="none">
            <AccordionButton
              _hover={{ bg: "blue.400", color: "white" }}
              py={2}
              px={4}
              borderRadius="md"
            >
              システム設定
              <AccordionIcon ml="auto" />
            </AccordionButton>
            <AccordionPanel p={4} bg="gray.700">
              <NavItem path="/admin" title="接続設定" index={0} />
              <NavItem
                path="/admin/system/image"
                title="カメラ設定"
                index={0}
              />
              <NavItem
                path="/admin/system/location"
                title="座標設定"
                index={0}
              />
            </AccordionPanel>
          </AccordionItem>

          {/* Dobot */}
          <AccordionItem border="none">
            <AccordionButton
              _hover={{ bg: "blue.400", color: "white" }}
              py={2}
              px={4}
              borderRadius="md"
            >
              Dobot
              <AccordionIcon ml="auto" />
            </AccordionButton>
            <AccordionPanel p={4} bg="gray.700">
              <NavItem path="/admin/dobot/setting" title="設定" index={3} />
              <NavItem path="/admin/dobot/operate" title="初期操作" index={3} />
            </AccordionPanel>
          </AccordionItem>

          {/* グリッパ */}
          <AccordionItem border="none">
            <AccordionButton
              _hover={{ bg: "blue.400", color: "white" }}
              py={2}
              px={4}
              borderRadius="md"
            >
              グリッパー
              <AccordionIcon ml="auto" />
            </AccordionButton>
            <AccordionPanel p={4} bg="gray.700">
              <NavItem path="/admin/glipper/setting" title="設定" index={3} />
              <NavItem
                path="/admin/glipper/operate"
                title="初期操作"
                index={3}
              />
            </AccordionPanel>
          </AccordionItem>
        </Accordion>
      </Box>
      {/* 右側のコンテンツエリア */}
      <Box flex="1" bg="gray.100" p={8}>
        {children}
      </Box>
    </Flex>
  );
};

export default AdminLayout;
