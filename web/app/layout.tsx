import type { Metadata } from "next";
import "./globals.css";

export const metadata: Metadata = {
  title: "Syrup Playground",
  description: "Interactive Syrup language playground",
};

export default function RootLayout({
  children,
}: {
  children: React.ReactNode;
}) {
  return (
    <html lang="en">
      <body>{children}</body>
    </html>
  );
}
