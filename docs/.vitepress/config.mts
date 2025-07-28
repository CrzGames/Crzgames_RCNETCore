import { defineConfig } from 'vitepress'

// https://vitepress.dev/reference/site-config
export default defineConfig({
  base: "/", // The base URL your site will be deployed at. You will need to set this if you plan to deploy your site under a sub path, for example GitHub pages.
  srcDir: './src' , // The directory where VitePress will resolve your markdown files from.
  lastUpdated: true,
  sitemap: { // Generate a sitemap for better SEO
    hostname: 'https://librcnet.crzcommon2.com'
  },
  title: "RCNET",
  locales: {
    // Config for Language = EN (default language)
    root: {
      description: "Game Server",
      label: 'English',
      lang: 'en',
      themeConfig: {
        nav: [
          { text: 'Home', link: '/' },
          { text: 'Guides', link: '/guides/getting-started/prerequisites' },
          { text: 'Platforms', link: '/platforms/introduction' },
          { text: 'API', link: '/api/rcnet' },
          { text: 'Release Notes', link: 'https://github.com/CrzGames/Crzgames_RCNETFramework/releases' },
        ],
        sidebar: {
          '/api/': [
            {
              text: 'RCNETCore API Reference',
              items: [
                { text: 'rcnet', link: '/api/rcnet' },
              ]
            },
          ],
          '/guides/': [
            {
              text: 'Guides',
              items: [
                { 
                  text: 'Getting Started', 
                  items: [
                    { text: 'Prerequisites', link: '/guides/getting-started/prerequisites' },
                    { text: 'Quick Start', link: '/guides/getting-started/quick-start' }
                  ] 
                },
                { 
                  text: 'Development', 
                  items: [
                    { text: 'Cycle Development', link: '/guides/development/development-cycle' },
                    { text: 'Updating Dependencies', link: '/guides/development/updating-dependencies' },
                  ] 
                },
                { text: 'Debugging', link: '/guides/debugging' },
                { text: 'Testing', link: '/guides/testing' },
                { text: 'Distribution', link: '/guides/distribution' },
              ]
            },
          ],
          '/platforms/': [
            {
              text: 'Platforms',
              items: [
                { text: 'Introduction', link: '/platforms/introduction' },
                { 
                  text: 'Linux', 
                  items: [
                    { text: 'Distribution', link: '/platforms/linux/distribution' },
                    { text: 'Project Linux', link: '/platforms/linux/project-linux' },
                  ] 
                }
              ]
            },
          ],
        }
      }
    },



    // Config for Language = FR
    fr: {
      description: "Server de jeu",
      label: 'Français',
      lang: 'fr',
      link: '/fr/',
      themeConfig: {
        nav: [
          { text: 'Accueil', link: '/fr/' },
          { text: 'Guides', link: '/fr/guides/getting-started/prerequisites' },
          { text: 'Projets', link: '/fr/projects/android' },
          { text: 'API', link: '/fr/api/rc2d' },
          { text: 'Notes de version', link: 'https://github.com/corentin35000/Crzgames_GameEngine/releases' },
        ],
        sidebar: {
          '/fr/api/': [
            {
              text: 'Référence de l\'API RC2DCore',
              items: [
                { text: 'rc2d', link: '/fr/api/rc2d' },
                { text: 'rc2d_audio', link: '/fr/api/rc2d_audio' },
                { text: 'rc2d_collision', link: '/fr/api/rc2d_collision' },
                { text: 'rc2d_event', link: '/fr/api/rc2d_event' },
                { text: 'rc2d_filesystem', link: '/fr/api/rc2d_filesystem' },
                { text: 'rc2d_font', link: '/fr/api/rc2d_font' },
                { text: 'rc2d_graphics', link: '/fr/api/rc2d_graphics' },
                { text: 'rc2d_image', link: '/fr/api/rc2d_image' },
                { text: 'rc2d_joystick', link: '/fr/api/rc2d_joystick' },
                { text: 'rc2d_keyboard', link: '/fr/api/rc2d_keyboard' },
                { text: 'rc2d_logger', link: '/fr/api/rc2d_logger' },
                { text: 'rc2d_math', link: '/fr/api/rc2d_math' },
                { text: 'rc2d_mouse', link: '/fr/api/rc2d_mouse' },
                { text: 'rc2d_net', link: '/fr/api/rc2d_net' },
                { text: 'rc2d_rres', link: '/fr/api/rc2d_rres' },
                { text: 'rc2d_spine', link: '/fr/api/rc2d_spine' },
                { text: 'rc2d_system', link: '/fr/api/rc2d_system' },
                { text: 'rc2d_timer', link: '/fr/api/rc2d_timer' },
                { text: 'rc2d_tweening', link: '/fr/api/rc2d_tweening' },
                { text: 'rc2d_window', link: '/fr/api/rc2d_window' },
              ]
            },
          ],
          '/fr/guides/': [
            {
              text: 'Guides',
              items: [
                { 
                  text: 'Commencer', 
                  items: [
                    { text: 'Conditions préalables', link: '/fr/guides/getting-started/prerequisites' },
                    { text: 'Démarrage rapide', link: '/fr/guides/getting-started/quick-start' }
                  ] 
                },
                { 
                  text: 'Développement', 
                  items: [
                    { text: 'Cycle de développement', link: '/fr/guides/development/development-cycle' },
                    { text: 'Dépendances', link: '/fr/guides/development/dependencies' }
                  ] 
                },
                { text: 'Assets', link: '/fr/guides/assets' },
                { text: 'Icônes', link: '/fr/guides/icons' },
                { text: 'Débogage', link: '/fr/guides/debugging' },
                { text: 'Test Unitaire', link: '/fr/guides/testing' },
                { text: 'Distribution', link: '/fr/guides/distribution' },
              ]
            },
          ],
          '/fr/platforms/': [
            {
              text: 'Platforms',
              items: [
                { 
                  text: 'Android', 
                  items: [
                    { text: 'Guides Google', link: '/fr/platforms/android/guides-google' },
                    { text: 'Playstore', link: '/fr/platforms/android/playstore' },
                    { text: 'New Release Android', link: '/fr/platforms/android/new-release-android' },
                    { text: 'Sign APK/AAB', link: '/fr/platforms/android/sign' },
                    { text: 'Specificity Android', link: '/fr/platforms/android/specificity' },
                    { text: 'Project Android', link: '/fr/platforms/android/project-android' },
                    { text: 'Must do', link: '/fr/platforms/android/must-do' },
                  ] 
                },
                { text: 'Android', link: '/fr/projects/guides-google' },
                { text: 'iOS', link: '/fr/projects/ios' },
                { text: 'Windows', link: '/fr/projects/windows' },
                { text: 'macOS', link: '/fr/projects/macos' },
                { text: 'Linux', link: '/fr/projects/linux' },
              ]
            },
          ],
        }
      }
    },
  },


  
  // Config for ALL LANGUAGES
  themeConfig: {
    search: {
      provider: 'local'
    },
    socialLinks: [
      { icon: 'github', link: 'https://github.com/CrzGames/Crzgames_RCNETFramework' }
    ],
    footer: {
      copyright: 'Copyright © 2024, Crzgames'
    }
  }
})