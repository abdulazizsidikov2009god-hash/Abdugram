// Smooth scroll to sections
function scrollToSection(sectionId) {
    const section = document.getElementById(sectionId);
    if (section) {
        section.scrollIntoView({ behavior: 'smooth' });
    }
}

// Navigation link active state
const navLinks = document.querySelectorAll('.nav-link');
const sections = document.querySelectorAll('section');

function updateActiveLink() {
    let current = '';
    sections.forEach(section => {
        const sectionTop = section.offsetTop;
        if (pageYOffset >= sectionTop - 200) {
            current = section.getAttribute('id');
        }
    });

    navLinks.forEach(link => {
        link.classList.remove('active');
        if (link.getAttribute('data-section') === current) {
            link.classList.add('active');
        }
    });
}

window.addEventListener('scroll', updateActiveLink);

// Hamburger menu
const hamburger = document.querySelector('.hamburger');
const navMenu = document.querySelector('.nav-menu');

if (hamburger) {
    hamburger.addEventListener('click', () => {
        navMenu.style.display = navMenu.style.display === 'flex' ? 'none' : 'flex';
        navMenu.style.position = 'absolute';
        navMenu.style.top = '70px';
        navMenu.style.left = '0';
        navMenu.style.width = '100%';
        navMenu.style.flexDirection = 'column';
        navMenu.style.backgroundColor = 'white';
        navMenu.style.padding = '20px';
        navMenu.style.boxShadow = '0 5px 10px rgba(0,0,0,0.1)';
    });
}

// Close menu when link is clicked
navLinks.forEach(link => {
    link.addEventListener('click', () => {
        if (navMenu && window.innerWidth <= 768) {
            navMenu.style.display = 'none';
        }
    });
});

// Download tracker
document.querySelectorAll('a[download]').forEach(link => {
    link.addEventListener('click', function(e) {
        const fileName = this.href.split('/').pop();
        console.log(`Starting download: ${fileName}`);
        
        // Track download in localStorage
        const downloads = JSON.parse(localStorage.getItem('abdugram_downloads') || '[]');
        downloads.push({
            file: fileName,
            timestamp: new Date().toISOString()
        });
        localStorage.setItem('abdugram_downloads', JSON.stringify(downloads));
    });
});

// Animations on scroll
const observerOptions = {
    threshold: 0.1,
    rootMargin: '0px 0px -100px 0px'
};

const observer = new IntersectionObserver((entries) => {
    entries.forEach(entry => {
        if (entry.isIntersecting) {
            entry.target.style.animation = 'fadeInUp 0.6s ease-out';
        }
    });
}, observerOptions);

// Observe feature cards
document.querySelectorAll('.feature-card').forEach(card => {
    observer.observe(card);
});

// Add fade-in animation
const style = document.createElement('style');
style.textContent = `
    @keyframes fadeInUp {
        from {
            opacity: 0;
            transform: translateY(30px);
        }
        to {
            opacity: 1;
            transform: translateY(0);
        }
    }
`;
document.head.appendChild(style);

// Version checker
const LATEST_VERSION = '1.0.0';
function checkVersion() {
    const userAgent = navigator.userAgent.toLowerCase();
    const isWindows = userAgent.includes('windows');
    const isWin64 = userAgent.includes('win64') || userAgent.includes('wow64');
    
    console.log(`Platform: ${isWin64 ? '64-bit' : '32-bit'} Windows`);
}

checkVersion();

// Download counter
function getDownloadStats() {
    const downloads = JSON.parse(localStorage.getItem('abdugram_downloads') || '[]');
    const stats = {};
    
    downloads.forEach(d => {
        stats[d.file] = (stats[d.file] || 0) + 1;
    });
    
    return stats;
}

// Security features display
const securityFeatures = [
    { icon: '🔐', title: 'AES-256 Encryption', description: 'Military-grade encryption for all messages' },
    { icon: '🔑', title: 'RSA 4096-bit', description: 'Secure key exchange protocol' },
    { icon: '⚡', title: 'End-to-End', description: 'Only sender and receiver can read' },
    { icon: '🛡️', title: 'HTTPS/TLS', description: 'Secure server connection' },
    { icon: '👤', title: 'JWT Auth', description: 'Token-based authentication' },
    { icon: '🔄', title: 'PBKDF2 Hashing', description: 'Secure password hashing' }
];

// Language support placeholder
const translations = {
    ru: {
        download: 'Скачать',
        features: 'Функции',
        security: 'Безопасность',
        'download-now': 'Скачать сейчас'
    },
    en: {
        download: 'Download',
        features: 'Features',
        security: 'Security',
        'download-now': 'Download Now'
    }
};

// Initialize
window.addEventListener('load', () => {
    console.log('Abdugram v' + LATEST_VERSION + ' loaded');
    updateActiveLink();
});