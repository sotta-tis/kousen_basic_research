// pages/index.tsx
import { Box, Button, Text } from "@chakra-ui/react";
import Layout from "../component/layouts/client/layout";
import SushiMenu from "../component/client/menu/SushiMenu";

const Home = ({ serverData }): JSX.Element => {
  return (
    <Layout>
      <SushiMenu count={serverData.sushi.count} menu={serverData.sushi.menu} />
    </Layout>
  );
};

export const getServerSideProps = async () => {
  try {
    // クエリパラメータでリクエストしたいURLを指定
    const urlToFetch = `${process.env.NEXT_PUBLIC_BACKEND_API_ENDPOINT}/`;
    const response = await fetch(urlToFetch);

    if (!response.ok) {
      throw new Error(`Failed to fetch data: ${response.statusText}`);
    }

    const data = await response.json();
    return {
      props: {
        serverData: data, // 取得したデータをpropsに渡す
      },
    };
  } catch (error) {
    return {
      props: {
        error: error.message,
      },
    };
  }
};
export default Home;
