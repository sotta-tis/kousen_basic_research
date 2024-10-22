import { welcomeModel } from "./welcomeModel";

export const Welcome = () => {
  const { res } = welcomeModel();

  return <>{JSON.stringify(res)}</>;
};
