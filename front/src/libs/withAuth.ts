import { GetServerSideProps } from "next";

export const withAuth = (gssp: GetServerSideProps) => {
  const validateAuth = async (context: Parameters<GetServerSideProps>[0]) => {
    const { req, res } = context;

    // Authorization ヘッダーから Basic 認証情報を取得
    const basicAuth = req.headers.authorization;

    if (!basicAuth) {
      res.statusCode = 401;
      res.setHeader("WWW-Authenticate", 'Basic realm="Secure Area"');
      res.end("Authentication required");
      return { props: {} };
    }

    // ユーザー名とパスワードを取得
    const authValue = basicAuth.split(" ")[1];
    const [user, password] = Buffer.from(authValue, "base64")
      .toString()
      .split(":");

    // 環境変数から正しいユーザー名とパスワードを取得
    const validUser = process.env.BASIC_AUTH_USER;
    const validPassword = process.env.BASIC_AUTH_PASSWORD;

    // 認証情報の確認
    if (user !== validUser || password !== validPassword) {
      res.statusCode = 401;
      res.setHeader("WWW-Authenticate", 'Basic realm="Secure Area"');
      res.end("Unauthorized");
      return { props: {} };
    }

    // 認証が成功した場合、gsspを実行
    return await gssp(context);
  };

  return validateAuth;
};
