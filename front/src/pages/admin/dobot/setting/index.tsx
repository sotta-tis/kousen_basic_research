// pages/index.tsx
import { Box, Button, Text } from "@chakra-ui/react";
import { GetServerSideProps } from "next";
import { withAuth } from "../../../../libs/withAuth";
import AdminLayout from "../../../../component/layouts/admin/adminLayout";
import DobotSetting from "../../../../component/admin/dobot/DobotSetting";
import { useState } from "react";

export const getServerSideProps: GetServerSideProps = withAuth(
  async (context) => {
    try {
      // クエリパラメータでリクエストしたいURLを指定
      const urlToFetch = `${process.env.NEXT_PUBLIC_BACKEND_API_ENDPOINT}/admin`;
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
  }
);

const Home = ({ serverData }): JSX.Element => {
  return (
    <AdminLayout pageKey={1}>
      <>{JSON.stringify(serverData)}</>
      <DobotSetting
        ipAddress={serverData.dobot.host}
        port={serverData.dobot.port}
      />
    </AdminLayout>
  );
};

export default Home;
