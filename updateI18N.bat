set PROJECT_DIR=%~dp0

lupdate  -recursive %PROJECT_DIR%src -ts %PROJECT_DIR%src\data\translations\en_US.ts
::linguist %PROJECT_DIR%src\data\translations\en_US.ts

lupdate  -recursive %PROJECT_DIR%src -ts %PROJECT_DIR%src\data\translations\zh_CN.ts
::linguist %PROJECT_DIR%src\data\translations\zh_CN.ts

lupdate  -recursive %PROJECT_DIR%src -ts %PROJECT_DIR%src\data\translations\zh_TW.ts
::linguist %PROJECT_DIR%src\data\translations\zh_TW.ts

pause