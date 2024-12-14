// 获取分类和内容
		const classifyItems = document.querySelectorAll('.product-classify li');
		// const classifyItemsSpan = document.querySelectorAll('.product-classify li span');
		const productContents = document.querySelectorAll('.product-content');
		
		
		// 添加点击事件
		classifyItems.forEach(item => {
			
			item.addEventListener('click', () => {
					
				// 移除所有分类的 active 状态
				classifyItems.forEach(el => el.classList.remove("active"));
				// 给当前分类添加 active 状态
				item.classList.add("active");
				
				// 获取目标内容的 ID
				const targetId = item.getAttribute('data-target');

				// 切换内容显示
				productContents.forEach(content => {
					content.classList.remove('active'); // 隐藏所有内容
					if (content.id === targetId) {
						content.classList.add('active'); // 显示目标内容
					}
				});
			});
		});
		  // 默认选中第一个分类
		  classifyItems[0].click();
