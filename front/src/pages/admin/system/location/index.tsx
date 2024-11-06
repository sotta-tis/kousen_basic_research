// pages/index.tsx
import { Box, Button, Text } from "@chakra-ui/react";
import { GetServerSideProps } from "next";
import { withAuth } from "../../../../libs/withAuth";
import AdminLayout from "../../../../component/layouts/admin/adminLayout";
import LocationSetting from "../../../../component/admin/system/LocationSetting";

const Home = ({ serverData }): JSX.Element => {
  return (
    <AdminLayout pageKey={0}>
      <LocationSetting
        x={serverData.dobot.x}
        y={serverData.dobot.y}
        z={serverData.dobot.z}
        r={serverData.dobot.r}
        xMax={serverData.location.zone.max_x}
        yMax={serverData.location.zone.max_y}
        xMin={serverData.location.zone.min_x}
        yMin={serverData.location.zone.min_y}
        zoneR={serverData.location.zone.r}
        zoneZ={serverData.location.zone.z}
        rlsR={serverData.location.release.r}
        rlsX={serverData.location.release.x}
        rlsY={serverData.location.release.y}
        rlsZ={serverData.location.release.z}
        dishX={serverData.location.dish.x}
        dishY={serverData.location.dish.y}
        dishR={serverData.location.dish.r}
        dishZ={serverData.location.dish.z}
        dishRlsR={serverData.location.dRelease.r}
        dishRlsX={serverData.location.dRelease.x}
        dishRlsY={serverData.location.dRelease.y}
        dishRlsZ={serverData.location.dRelease.z}
      />
    </AdminLayout>
  );
};

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

export default Home;
