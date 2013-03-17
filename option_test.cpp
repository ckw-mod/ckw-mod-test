#include "gtest/gtest.h"
#include "option.h"
#include "option.cpp"

#include <windows.h>
#include <shlwapi.h>

TEST(OptionTest, lookupColor) {
	bool b;
	COLORREF c;

	b = lookupColor("AliceBlue", c);
	EXPECT_TRUE(b);
	EXPECT_EQ(RGB(0xF0,0xF8,0xFF), c);

	b = lookupColor("Dark Green", c);
	EXPECT_TRUE(b);
	EXPECT_EQ(RGB(0x00,0x64,0x00), c);

	b = lookupColor("#446688", c);
	EXPECT_TRUE(b);
	EXPECT_EQ(RGB(0x44,0x66,0x88), c);

	b = lookupColor("#468", c);
	EXPECT_TRUE(b);
	EXPECT_EQ(RGB(0x40,0x60,0x80), c);

	b = lookupColor("#1234", c);
	EXPECT_TRUE(b);
	EXPECT_EQ(RGB(0x10,0x20,0x30), c);

	b = lookupColor("#123456789", c);
	EXPECT_TRUE(b);
	EXPECT_EQ(RGB(0x12,0x45,0x78), c);
}

TEST(OptionTest, lookupColorError) {
	bool b;
	COLORREF c;

	testing::internal::CaptureStderr();
	b = lookupColor("xxxx", c);
	EXPECT_FALSE(b);
	EXPECT_STREQ("ERROR: invalid color. \"xxxx\"\n",
		     testing::internal::GetCapturedStderr().c_str());

	testing::internal::CaptureStderr();
	b = lookupColor("#00", c);
	EXPECT_FALSE(b);
	EXPECT_STREQ("ERROR: invalid color. \"#00\"\n",
		     testing::internal::GetCapturedStderr().c_str());
}

TEST(OptionTest, lookupBoolean) {
	EXPECT_TRUE(lookupBoolean("true"));
	EXPECT_TRUE(lookupBoolean("Yes"));
	EXPECT_TRUE(lookupBoolean("ON"));
	EXPECT_TRUE(lookupBoolean("1"));
//	EXPECT_TRUE(lookupBoolean("t"));
//	EXPECT_TRUE(lookupBoolean("y"));

	EXPECT_FALSE(lookupBoolean("false"));
	EXPECT_FALSE(lookupBoolean("No"));
	EXPECT_FALSE(lookupBoolean("OFF"));
	EXPECT_FALSE(lookupBoolean("0"));
	EXPECT_FALSE(lookupBoolean("nil"));
	EXPECT_FALSE(lookupBoolean("n"));
}


TEST(OptionTest, geometry) {
	{
		ckOpt opt;
		opt.geometry("80x40+12+34");
		EXPECT_EQ(80, opt.getWinCharW());
		EXPECT_EQ(40, opt.getWinCharH());
		EXPECT_TRUE(opt.isWinPos());
		EXPECT_EQ(12, opt.getWinPosX());
		EXPECT_EQ(34, opt.getWinPosY());
	}
	{
		ckOpt opt;
		opt.geometry("20x50");
		EXPECT_EQ(20, opt.getWinCharW());
		EXPECT_EQ(50, opt.getWinCharH());
		EXPECT_FALSE(opt.isWinPos());
	}
	{
		ckOpt opt;
		opt.geometry("x45");
		EXPECT_EQ(1, opt.getWinCharW());
		EXPECT_EQ(45, opt.getWinCharH());
		EXPECT_FALSE(opt.isWinPos());
	}
	{
		ckOpt opt;
		opt.geometry("-98");
		EXPECT_TRUE(opt.isWinPos());
		EXPECT_EQ(-98-1, opt.getWinPosX());
		EXPECT_EQ(0, opt.getWinPosY());
	}
}

TEST(OptionTest, extract) {
	std::string app;
	std::string name;
	std::string value;

	char line1[] = "Ckw*title1:ckw[cmd]";
	extract(line1, app, name, value);
	EXPECT_STREQ("Ckw", app.c_str());
	EXPECT_STREQ("title1", name.c_str());
	EXPECT_STREQ("ckw[cmd]", value.c_str());

	char line2[] = "Ckw*title2: ckw[cmd]";
	extract(line2, app, name, value);
	EXPECT_STREQ("Ckw", app.c_str());
	EXPECT_STREQ("title2", name.c_str());
	EXPECT_STREQ("ckw[cmd]", value.c_str());

	char line3[] = "Ckw*title3:\t\tckw[cmd]";
	extract(line3, app, name, value);
	EXPECT_STREQ("Ckw", app.c_str());
	EXPECT_STREQ("title3", name.c_str());
	EXPECT_STREQ("ckw[cmd]", value.c_str());

	char line4[] = "KTerm*VT100*geometry: 90x45";
	extract(line4, app, name, value);
	EXPECT_STREQ("KTerm", app.c_str());
	EXPECT_STREQ("VT100*geometry", name.c_str());
	EXPECT_STREQ("90x45", value.c_str());

	char line5[] = "Ckw*title5:";
	extract(line5, app, name, value);
	EXPECT_STREQ("Ckw", app.c_str());
	EXPECT_STREQ("title5", name.c_str());
	EXPECT_STREQ("", value.c_str());
}

TEST(OptionTest, usagefmtL) {
	testing::internal::CaptureStdout();
	usage_fmtL("fontSize", "number");
	EXPECT_STREQ("  fontSize:            number\n",
		     testing::internal::GetCapturedStdout().c_str());

	testing::internal::CaptureStdout();
	usage_fmtL("foreground", "color");
	EXPECT_STREQ("  foreground:          color\n",
		     testing::internal::GetCapturedStdout().c_str());

	testing::internal::CaptureStdout();
	usage_fmtL("geometry", "string");
	EXPECT_STREQ("  geometry:            string\n",
		     testing::internal::GetCapturedStdout().c_str());

	testing::internal::CaptureStdout();
	usage_fmtL("scrollRight", "boolean");
	EXPECT_STREQ("  scrollRight:         boolean\n",
		     testing::internal::GetCapturedStdout().c_str());
}

TEST(OptionTest, usagefmtS) {
	testing::internal::CaptureStdout();
	usage_fmtS("fs", "number", "text font size");
	EXPECT_STREQ("  -fs <number>         : text font size\n",
		     testing::internal::GetCapturedStdout().c_str());

	testing::internal::CaptureStdout();
	usage_fmtS("fg", "color", "foreground color");
	EXPECT_STREQ("  -fg <color>          : foreground color\n",
		     testing::internal::GetCapturedStdout().c_str());

	testing::internal::CaptureStdout();
	usage_fmtS("g", "string", "window layout. ( ex. 80x24+0+0 )");
	EXPECT_STREQ("  -g <string>          : window layout. ( ex. 80x24+0+0 )\n",
		     testing::internal::GetCapturedStdout().c_str());

	testing::internal::CaptureStdout();
	usage_fmtS("sr", "boolean", "turn on/off scrollbar right");
	EXPECT_STREQ("  -/+sr                : turn on/off scrollbar right\n",
		     testing::internal::GetCapturedStdout().c_str());
}

TEST(OptionTest, cmdsMake) {
	{
		char *argv[] = {"sh.exe", "--login", "-i"};
		int argc = sizeof(argv) / sizeof(argv[0]);
		ckOpt opt;
		opt.cmdsMake(argc, argv);
//		EXPECT_STREQ("sh.exe --login -i", opt.getCmd());
		EXPECT_STREQ("sh.exe --login -i ", opt.getCmd());
	}

	{
		char *argv[] = {"cmd.exe", "foo=bar", "hoge fuga"};
		int argc = sizeof(argv) / sizeof(argv[0]);
		ckOpt opt;
		opt.cmdsMake(argc, argv);
//		EXPECT_STREQ("cmd.exe \"foo=bar\" \"hoge fuga\"", opt.getCmd());
		EXPECT_STREQ("cmd.exe \"foo=bar\" \"hoge fuga\" ", opt.getCmd());
	}

}

TEST(OptionTest, setOption) {
	{
		// 設定ファイルによる設定
		ckOpt opt;
		opt.setOption("fontSize", "20", true);
		EXPECT_EQ(20, opt.getFontSize());

		opt.setOption("foreground", "#332211", true);
		EXPECT_EQ(RGB(0x33,0x22,0x11), opt.getColorFg());

		opt.setOption("geometry", "100x20+0-0", true);
		EXPECT_EQ(100, opt.getWinCharW());
		EXPECT_EQ(20, opt.getWinCharH());
		EXPECT_TRUE(opt.isWinPos());
		EXPECT_EQ(0, opt.getWinPosX());
		EXPECT_EQ(-1, opt.getWinPosY());

		opt.setOption("scrollRight", "0", true);
		EXPECT_FALSE(opt.isScrollRight());
	}
	{
		// ロングオプションによる設定
		ckOpt opt;
		opt.setOption("--fontSize", "20", false);
		EXPECT_EQ(20, opt.getFontSize());

		opt.setOption("--foreground", "#332211", false);
		EXPECT_EQ(RGB(0x33,0x22,0x11), opt.getColorFg());

		opt.setOption("--geometry", "100x20+0-0", false);
		EXPECT_EQ(100, opt.getWinCharW());
		EXPECT_EQ(20, opt.getWinCharH());
		EXPECT_TRUE(opt.isWinPos());
		EXPECT_EQ(0, opt.getWinPosX());
		EXPECT_EQ(-1, opt.getWinPosY());

		opt.setOption("--scrollRight", "0", false);
		EXPECT_FALSE(opt.isScrollRight());
	}
	{
		// ショートオプションによる設定
		ckOpt opt;
		opt.setOption("-fs", "20", false);
		EXPECT_EQ(20, opt.getFontSize());

		opt.setOption("-fg", "#332211", false);
		EXPECT_EQ(RGB(0x33,0x22,0x11), opt.getColorFg());

		opt.setOption("-g", "100x20+0-0", false);
		EXPECT_EQ(100, opt.getWinCharW());
		EXPECT_EQ(20, opt.getWinCharH());
		EXPECT_TRUE(opt.isWinPos());
		EXPECT_EQ(0, opt.getWinPosX());
		EXPECT_EQ(-1, opt.getWinPosY());

		opt.setOption("+sr", NULL, false);
		EXPECT_FALSE(opt.isScrollRight());
	}
}
