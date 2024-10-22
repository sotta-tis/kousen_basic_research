import { customFetch } from "../../libs/customFetch";

export const welcomeModel = () => {
  console.log("API Endpoint:", process.env.REACT_APP_BACKEND_API_ENDPOINT);
  const { res } = customFetch("/");

  return {
    res,
  };
};
