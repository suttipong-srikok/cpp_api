#include <gtest/gtest.h>
#include "../src/utils.h"

TEST(UserToJsonTest, CorrectlySerializesUser) {
    User user;
    user.id = 1;
    user.name = "Test User";
    user.email = "test@example.com";

    crow::json::wvalue json_user = user_to_json(user);

                    EXPECT_EQ(json_user.dump(), "{\"email\":\"test@example.com\",\"name\":\"Test User\",\"id\":1}");
}


