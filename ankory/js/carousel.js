const carousel = document.getElementById('carousel');
const slides = document.querySelectorAll('.carousel-item');
const slideHeight = slides[0].offsetHeight;
let currentIndex = 1; // 从实际内容的第一张开始
let autoPlayInterval;
let isSwitching = false;  // 标记是否正在切换中

function updateCarousel(index, transition = true) {
  if (!transition) {
    carousel.style.transition = 'none';
  } else {
    carousel.style.transition = 'transform 0.5s ease-in-out';
  }
  carousel.style.transform = `translateY(-${slideHeight * index}px)`;
}

function handleLoop() {
  if (currentIndex === 0) {
    currentIndex = slides.length - 2; // 跳到实际最后一张
    updateCarousel(currentIndex, false);
  } else if (currentIndex === slides.length - 1) {
    currentIndex = 1; // 跳到实际第一张
    updateCarousel(currentIndex, false);
  }
}

function nextSlide() {
  if (isSwitching) return;  // 如果正在切换，阻止多次点击
  isSwitching = true;  // 标记正在切换
  currentIndex++;
  updateCarousel(currentIndex);
  setTimeout(() => {
    handleLoop();
    isSwitching = false;  // 切换完成，允许再次点击
  }, 500); // 设置超时时间与过渡时间一致
}

function prevSlide() {
  if (isSwitching) return;  // 如果正在切换，阻止多次点击
  isSwitching = true;  // 标记正在切换
  currentIndex--;
  updateCarousel(currentIndex);
  setTimeout(() => {
    handleLoop();
    isSwitching = false;  // 切换完成，允许再次点击
  }, 500); // 设置超时时间与过渡时间一致
}

function startAutoPlay() {
  autoPlayInterval = setInterval(nextSlide, 3000);
}

function stopAutoPlay() {
  clearInterval(autoPlayInterval);
}

document.getElementById('prevBtn').addEventListener('click', () => {
  stopAutoPlay();
  prevSlide();
  startAutoPlay();
});

document.getElementById('nextBtn').addEventListener('click', () => {
  stopAutoPlay();
  nextSlide();
  startAutoPlay();
});

// 初始化
updateCarousel(currentIndex, false);
startAutoPlay();
