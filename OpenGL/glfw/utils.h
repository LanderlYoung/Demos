/**
* <pre>
* Author: taylorcyang@tencent.com
* Date:   2019-08-05
* Time:   13:12
* Life with Passion, Code with Creativity.
* </pre>
*/

#pragma once

#define DISALLOW_COPY(CLASS_NAME) CLASS_NAME(CLASS_NAME&) = delete

#define DISALLOW_ASSIGN(CLASS_NAME) CLASS_NAME& operator=(CLASS_NAME&) = delete

#define DISALLOW_COPY_ASSIGN(CLASS_NAME) DISALLOW_COPY(CLASS_NAME); DISALLOW_ASSIGN(CLASS_NAME)
