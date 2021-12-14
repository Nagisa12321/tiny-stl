# Tiny STL
My small implementation of stl

### ➡ Allocator
- 位置`tinystl_alloc.h`
1. 符合STL接口规范的`allocator`
	- 思想: 将对象的创建、销毁分为四部分: 申请内存, 执行构造函数, 执行析构函数, 释放内存
	- 申请内存: malloc(...)
	- 执行构造函数: c++ 自带 new in place 手法
	- 执行析构函数: 直接调用析构函数而不是delete
	- 释放内存: free(...)
	
2. 容器内部使用的alloc: `__default_alloc_template` 和 `__malloc_alloc_template`. 并且使用`simple_alloc` 作为接口使用
	- `__malloc_alloc_template` 行为类似于allocator. 
	- `__default_alloc_template` 采用了free list + memory pool 技术, 详见代码. 
### ➡ iterator traits and type traits
### ➡ tinystd::vector
### ➡ tinystd::list
