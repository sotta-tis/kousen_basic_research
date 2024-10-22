// pages/index.tsx
import { Box, Button, Text } from "@chakra-ui/react";
import Layout from "../component/layouts/client/layout";
import OrderItems from "../component/client/menu/SushiMenu";

const Home = (): JSX.Element => {
  return (
    <Layout>
      <OrderItems />
    </Layout>
  );
};

export default Home;
