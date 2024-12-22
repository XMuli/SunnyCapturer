import { defineConfig } from 'vitepress'

export default defineConfig({
  title: "SunnyCapturer",
  description: "SunnyCapturer / Pinning / OCR / Image Translation / Capture / Linux / 截图 / 推荐", // seo 的优化
  // base: '/SunnyPages/',    // https://xmuli.tech/SunnyPages 且去掉 CNAME 文件后
  base: '/',                  // https://sunny.xmuli.tech 和 CNAME 文件
  head: [['link', { rel: 'icon', href: 'images/favicon.ico' }]],

  locales: {
    root: {
      selectText: 'Languages',
      label: 'English',
      lang: 'en-US',
      link: '/'
    },
    zh: {
      selectText: '选择语言',
      label: '简体中文',
      lang: 'zh-CN',
      link: '/zh/'
    }
  },

  themeConfig: {
    siteTitle: 'Sunny',                 // 站点标题
    logo: '/images/logo/logo.svg',      // 站点 logo

    nav: [
      { text: 'Home', link: '/' },
      { text: '⚔Platform', link: '/article/supportp_platform' },  // 文件夹不指定则默认加载 index.md
      { text: 'Feedback', link: '/article/suggest_feedback' },
      {
        text: 'Docs', items: [
          { text: 'Development Experience', link: '/article/how_develop_it' },
          { text: 'Code open Source', link: 'https://github.com/SunnyScreenshot' },
          { text: 'Change Log', link: 'https://txc.qq.com/products/649489/change-log' },
          // { text: 'Wiki', link: 'https://github.com/XMuli/SunnyPages/wiki' },
        ],
      },
      // {
      //   text: 'More Product', items: [
      //     { text: 'QtExamples', link: 'https://github.com/XMuli/QtExamples' },
      //     { text: 'ThinkMate', link: 'https://thinkymate.com' },
      //     { text: 'ChineseChess', link: 'https://github.com/XMuli/ChineseChess' },
      //     { text: 'FLIPPED', link: 'https://flipped.xmuli.tech' },
      //     { text: 'ShotX', link: 'https://github.com/XMuli/shotx' },
      //   ]
      // },
      { text: 'Internalization', link: 'https://github.com/SunnyScreenshot/translations' }
    ],

    sidebar: [
      {
        text: 'Catalogue',
        items: [
          { text: 'Download', link: 'https://github.com/XMuli/SunnyPages/releases' },
          { text: 'Feature', link: '/feature' },
          { text: 'Platform', link: '/article/supportp_platform' },
          { text: 'Feedback', link: '/article/suggest_feedback' },
          { text: 'Development Experience', link: '/article/how_develop_it' },
          { text: 'Acknowledgement', link: '/acknowledgement' },
        ]
      },
      {
        text: '目录',
        items: [
          { text: '官网下载', link: 'https://github.com/XMuli/SunnyPages/releases' },
          { text: '特色功能', link: '/feature' },
          { text: '架构平台', link: '/article/supportp_platform' },
          { text: '建议反馈', link: '/article/suggest_feedback' },
          { text: '开发经验', link: '/article/how_develop_it' },
          { text: '鸣    谢', link: '/acknowledgement' },
        ]
      }
    ],

    socialLinks: [
      // { icon: 'twitter', link: 'https://twitter.com/xmulite' },
      { icon: 'github', link: 'https://github.com/XMuli/SunnyPages' },
      {
        icon: {
          svg: '<svg t="1716911889837" class="icon" viewBox="0 0 1024 1024" version="1.1" xmlns="http://www.w3.org/2000/svg" p-id="2610" width="32" height="32"><path d="M679.424 746.861714l84.004571-395.995428c7.424-34.852571-12.580571-48.566857-35.437714-40.009143l-493.714286 190.281143c-33.718857 13.129143-33.133714 32-5.705142 40.557714l126.281142 39.424 293.156572-184.576c13.714286-9.142857 26.294857-3.986286 16.018286 5.156571l-237.129143 214.272-9.142857 130.304c13.129143 0 18.870857-5.705143 25.709714-12.580571l61.696-59.428571 128 94.281142c23.442286 13.129143 40.009143 6.290286 46.299428-21.723428zM1024 512c0 282.843429-229.156571 512-512 512S0 794.843429 0 512 229.156571 0 512 0s512 229.156571 512 512z" fill="#515151" p-id="2611"></path></svg>'
        },
        link: 'https://t.me/xmuli',
        ariaLabel: 'Telegram'
      },
      {
        icon: {
          svg: '<svg t="1716912142377" class="icon" viewBox="0 0 1024 1024" version="1.1" xmlns="http://www.w3.org/2000/svg" p-id="8228" width="32" height="32"><path d="M869.946746 524.53484q25.141103 61.709981 42.282765 116.563298 30.854991 103.992746 30.283602 151.989398t-10.856386 47.996652q-20.569994-4.556825-53.710539-51.424985T841.37731 735.948664q0 47.996652-24.569715 102.849969t-77.137476 100.564414a354.603836 354.603836 0 0 1 41.139987 14.284718 181.058799 181.058799 0 0 1 37.711656 21.141382q12.584836 10.284997 14.284717 19.427216c1.142777 6.099575 0.571389 10.6564-1.714166 13.713329q-22.855549 17.141661-139.418846 15.998884t-179.416056-7.999442q-64.009821 6.856665-179.987445 7.999442t-139.990236-15.998884c-1.528465-3.05693-1.899867-7.613755-1.142777-13.713329s5.713887-12.570552 14.856107-19.427216a151.789412 151.789412 0 0 1 37.140266-21.141382 383.844654 383.844654 0 0 1 41.711376-14.284718q-52.553477-45.711097-77.708865-100.564414t-25.141103-102.849969Q178.566401 742.805329 146.040099 789.659203T92.858094 841.084188q-10.284997 0-11.427774-47.425264t29.712213-152.560786q18.284439-55.981809 42.854154-117.706076t37.140266-93.707749V394.258213Q189.994176 241.126038 266.031729 123.419962T512.300265 0q169.088205 5.713887 245.697147 123.419962t75.42331 270.838251l-1.142777 35.4261z" p-id="8229" fill="#515151"></path></svg>'
        },
        link: 'https://qm.qq.com/cgi-bin/qm/qr?authKey=5pYNrJL7%2F8biKzT5LMj8dbjkpPvUvdLVbAOcNTydiqTDNc49yg0wtVcub8Cu3Pqa&k=OluWZhjVMhwP-6RO9Y7FFkJcXGiS4CVk&noverify=0',
        ariaLabel: 'QQ'
      }
    ],

    // footer: {
    // message: 'Copyright 2023-2024 XMuli',
    // copyright: 'Powered by <a href="https://vitepress.dev" target="_blank" rel="noopener">VitePress</a>'
    // }
  }
})
